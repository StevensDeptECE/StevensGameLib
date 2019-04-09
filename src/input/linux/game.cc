#include "game.hh"
#include "dbkbd.hh"
#include "player.hh"
#include "bullet.hh"
#include "asteroid.hh"

#include <unistd.h>
#include <vector>
#include <ctime>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const float PI = glm::pi<float>();

int Game::Width = 0;
int Game::Height = 0;

Inputs *inp;

/*** Entities ***/
std::vector<Player> players;
std::vector<Bullet> bullets;
std::vector<Asteroid> asteroids;

/*** Control ***/
// Put this stuff in a struct
// or something useful
int *space_counter;
int *prev_space;

void create_bullet(int id);
void create_asteroid();

Game::Game(int width, int height)
{
	State = GAME_ACTIVE;
	Width = width;
	Height = height;
	inp = new Inputs;
}

Game::~Game()
{
	std::cout << "Goodbye\n" << std::endl;
	delete inp;
}

void Game::init()
{
	// Create players
	int num_players = inp->keyboard_count(); 
	std::cout << "Keyboards: " << num_players << std::endl;

	// Control arrays
	space_counter = new int[num_players];
	prev_space = new int[num_players];

	for (int j = 0; j < num_players; ++j) {
		players.push_back(Player((double)rand() / RAND_MAX - .1, (double)rand() / RAND_MAX - .1));
		space_counter[j] = 0;
		prev_space[j] = 0;
	}

	// Player shaders, VAOs, VBOs, EBOs, etc.
	for (int j = 0; j < num_players; ++j) {
		players[j].create_shader();
	}
}

int Game::check_collision(Asteroid obj_a, Bullet obj_b)
{
	float obj_a_x = obj_a.x + 1.0 - obj_a.size/Game::Width;
	float obj_a_y = obj_a.y + 1.0 + obj_a.size/Game::Height;

	float obj_b_x = obj_b.x + 1.0 - obj_b.size/Game::Width;
	float obj_b_y = obj_b.y + 1.0 + obj_b.size/Game::Height;

	int collision_x = (obj_a_x + 2*obj_a.size/Game::Width) >= obj_b_x && obj_b_x + 2*obj_b.size/Game::Width >= obj_a_x;
	int collision_y = (obj_a_y - 2*obj_a.size/Game::Height) <= obj_b_y && obj_b_y - 2*obj_b.size/Game::Height <= obj_a_y;

	return collision_x && collision_y;
}

int Game::check_collision(Asteroid obj_a, Player obj_b)
{
	float obj_a_x = obj_a.x + 1.0 - obj_a.size/Game::Width;
	float obj_a_y = obj_a.y + 1.0 + obj_a.size/Game::Height;

	float obj_b_x = obj_b.x + 1.0 - obj_b.size/Game::Width;
	float obj_b_y = obj_b.y + 1.0 + obj_b.size/Game::Height;

	int collision_x = (obj_a_x + 2*obj_a.size/Game::Width) >= obj_b_x && obj_b_x + 2*obj_b.size/Game::Width >= obj_a_x;
	int collision_y = (obj_a_y - 2*obj_a.size/Game::Height) <= obj_b_y && obj_b_y - 2*obj_b.size/Game::Height <= obj_a_y;

	return collision_x && collision_y;
}
static int hit = 0;
void Game::physics(float dt)
{
	// Collision detection
	// For each bullet, check if collision
	for (Asteroid &asteroid : asteroids) {
		for (Bullet &bullet : bullets) {
			if (check_collision(asteroid, bullet)) {
				bullet.do_remove();
				asteroid.do_remove();
			}
		}
		for (Player &player : players) {
			if (check_collision(asteroid, player)) {
					std::cout << "hit!" << std::endl;
					asteroid.do_remove();
			}
		}
	}
}

static float accumulator = 0;
void Game::update(float dt)
{
	physics(dt);

	int i;
	for (i = 0; i < players.size(); ++i) {
		players[i].update(dt);
	}
	for (i = 0; i < bullets.size(); ) {
		bullets[i].update(dt);
		if (bullets[i].should_remove()) {
			bullets.erase(bullets.begin() + i);
		} else {
			++i;
		}
	}
	for (i = 0; i < asteroids.size(); ) {
		asteroids[i].update(dt);
		if (asteroids[i].should_remove()) {
			asteroids.erase(asteroids.begin() + i);
		} else {
			++i;
		}
	}

	if (accumulator >= 0.3) {
		create_asteroid();
		accumulator = 0;
	}
	accumulator += dt;
}

void Game::render()
{
	// Draw square per player
	for (int j = 0; j < players.size(); ++j) {
		players[j].render();
	}

	for (int j = 0; j < bullets.size(); ++j) {
		bullets[j].render();
	}

	for (int j = 0; j < asteroids.size(); ++j) {
		asteroids[j].render();
	}
}

void Game::process_input(float dt)
{
	Inputs *i = inp;
	for (int j = 0; j < i->keyboard_count(); ++j) {
		// translation
		if (i->get_key(j, 17))
			players[j].move(0, .5 * dt);
		if (i->get_key(j, 30))
			players[j].move(-.5 * dt, 0);
		if (i->get_key(j, 32))
			players[j].move(.5 * dt, 0);
		if (i->get_key(j, 31))
			players[j].move(0, -.5 * dt);

		// rotation
		if (i->get_key(j, 105))
			players[j].rotate(3 * dt);
		if (i->get_key(j, 106))
			players[j].rotate(-3 * dt);

		// space
		if (i->get_key(j, 57)) {
			if (!prev_space[j] || space_counter[j] == 05) {
				create_bullet(j);
				space_counter[j] = 0;
			}
			prev_space[j] = 1;
			++space_counter[j];
		} else if (!i->get_key(j, 57)) {
			prev_space[j] = 0;
			space_counter[j] = 0;
		}
	}
}

void create_bullet(int id)
{
	// Upwards is 0 rad, but in reality it's pi/2 rad
	// r = Player::size / 2 / screenwidth
	// offset = center of player
	float r = 75.0 / 2.0 / Game::Width;
	float x = cos(players[id].angle+PI/2.0) * r + (players[id].x + r);
	float y = sin(players[id].angle+PI/2.0) * r + (players[id].y - r);
	bullets.push_back(Bullet(x, y, players[id].angle));
	bullets[bullets.size() - 1].create_shader();
}

void create_asteroid()
{
	float x = (float)rand() / RAND_MAX * 2 - 1;
	float y = (float)rand() / RAND_MAX * 2 - 1;
	float angle = (float)rand() / RAND_MAX * PI;
	if (asteroids.size() < 25) {
		asteroids.push_back(Asteroid(x, y, angle));
		asteroids[asteroids.size() - 1].create_shader();
	}
}
