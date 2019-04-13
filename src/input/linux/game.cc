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

Game::Game(int width, int height, float act) : accumulator(0), asteroid_creation_time(act), score(0), deaths(0)
{
	Height = height;
	Width = width;
	State = GAME_ACTIVE;
	inp = new Inputs;
}

Game::~Game()
{
	std::cout << "Goodbye\n" << std::endl;
	delete inp;
	delete[] space_counter;
	delete[] prev_space;
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

bool Game::check_collision(const GameObject &a, const GameObject &b) const
{
	float a_x = a.x + 1.0 - a.size/Width;
	float a_y = a.y + 1.0 + a.size/Height;

	float b_x = b.x + 1.0 - b.size/Width;
	float b_y = b.y + 1.0 + b.size/Height;

	int collision_x = (a_x + 2*a.size/Width) >= b_x && b_x + 2*b.size/Width >= a_x;
	int collision_y = (a_y - 2*a.size/Height) <= b_y && b_y - 2*b.size/Height <= a_y;

	return collision_x && collision_y;
}

void Game::physics(float dt)
{
	// Collision detection
	// For each bullet, check if collision
	for (Asteroid &asteroid : asteroids) {
		for (Bullet &bullet : bullets) {
			if (check_collision(asteroid, bullet)) {
				bullet.do_remove();
				asteroid.do_remove();
				std::cout << "Score: " << ++score << "\n";
			}
		}
		for (Player &player : players) {
			if (check_collision(asteroid, player)) {
					std::cout << "Hit " << ++deaths << " times\n";
					asteroid.do_remove();
			}
		}
	}
}

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

	if (accumulator <= 0) {
		create_asteroid();
		accumulator = asteroid_creation_time;
	}
	accumulator -= dt;
}

void Game::render()
{
	// Draw square per player
	for (int j = 0; j < players.size(); ++j) {
		players[j].set_transform();
		players[j].render();
	}

	for (int j = 0; j < bullets.size(); ++j) {
		bullets[j].set_transform();
		bullets[j].render();
	}

	for (int j = 0; j < asteroids.size(); ++j) {
		asteroids[j].set_transform();
		asteroids[j].render();
	}
}

void Game::process_input(float dt)
{
	Inputs *i = inp;
	for (int j = 0; j < i->keyboard_count(); ++j) {
		// translation
		if (i->get_key(j, 17))
			players[j].move(0, .7 * dt);
		if (i->get_key(j, 30))
			players[j].move(-.7 * dt, 0);
		if (i->get_key(j, 32))
			players[j].move(.7 * dt, 0);
		if (i->get_key(j, 31))
			players[j].move(0, -.7 * dt);

		// rotation
		if (i->get_key(j, 105))
			players[j].rotate(5 * dt);
		if (i->get_key(j, 106))
			players[j].rotate(-5 * dt);

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

void Game::create_bullet(int id)
{
	// Upwards is 0 rad, but in reality it's pi/2 rad
	// r = Player::size / 2 / screenwidth
	// offset = center of player
	float r = 75.0 / 2.0 / Game::Width;
	float x = cos(players[id].angle+PI/2.0) * r + (players[id].x);// + r);
	float y = sin(players[id].angle+PI/2.0) * r + (players[id].y);// - r);
	bullets.push_back(Bullet(x, y, players[id].angle));
	bullets[bullets.size() - 1].create_shader();
	bullets.push_back(Bullet(x, y, players[id].angle-PI/6.0));
	bullets[bullets.size() - 1].create_shader();
	bullets.push_back(Bullet(x, y, players[id].angle+PI/6.0));
	bullets[bullets.size() - 1].create_shader();
}

void Game::create_asteroid()
{
	float x = (float)rand() / RAND_MAX + 1;
	float y = (float)rand() / RAND_MAX;
	float angle = (float)rand() / RAND_MAX * PI * 2;
	float size = (float)(rand() % 30) + 30.0;
	if (asteroids.size() < 10) {
		asteroids.push_back(Asteroid(x, y, angle, size));
		asteroids[asteroids.size() - 1].create_shader();
	}
}
