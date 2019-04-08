#include "game.hh"
#include "dbkbd.hh"
#include "player.hh"
#include "bullet.hh"

#include <unistd.h>
#include <vector>
#include <ctime>
#include <iostream>
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const float PI = glm::pi<float>();

Inputs *inp;

/*** Entities ***/
std::vector<Player> players;
std::vector<Bullet> bullets;

/*** Control ***/
bool play = true;
int bulletId = -1;
// Put this stuff in a struct
// or something useful
int *space_counter;
int *prev_space;

void createBullet(int id);

Game::Game(int width, int height)
{
	State = GAME_ACTIVE;
	Width = width;
	Height = height;
	inp = new Inputs;
}

Game::~Game()
{
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

int Game::check_collision(Bullet bullet, Player player)
{
	// origin of bullet is origin of circle
	// origin of player is top left(?)

	// radius is 10/800

	//trans = glm::translate(trans, glm::vec3(x + dist * cos(angle), y + dist * sin(angle), 1.0));

	float bullet_x = bullet.x + 1.0 + bullet.dist * cos(bullet.angle) - 10/800.0;
	float bullet_y = bullet.y + 1.0 + bullet.dist * sin(bullet.angle) + 10/800.0;

	float player_x = player.x + 1.0;
	float player_y = player.y + 1.0;

	//int collision_x = (player.x + player.size/800.0 >= bullet_x) && (bullet_x + 10/800.0 >= player.x);
	int collision_x = (bullet_x + 10/800.0) >= player_x && player_x + player.size/800.0 >= bullet_x;
	int collision_y = (bullet_y - 10/800.0) <= player_y && player_y - player.size/800.0 <= bullet_y;

	//int collision_y = (player.y - player.size/800.0 >= bullet_y) && (bullet_y + 10/800.0 >= player.y);

	//printf("player.y: %f\n", player.y);
	//printf("p_x: %f  p_y: %f  b_x: %f  b_y: %f\n", player.x , player.y , bullet_x, bullet_y);
	//fflush(stdout);

	return collision_x && collision_y;


}

static int hit = 0;
void Game::physics(float dt)
{
	// Collision detection
	// For each bullet, check if collision
	for (Bullet &bullet : bullets) {
		int i = 0;
		for (Player &player : players) {
			if (check_collision(bullet, player)) {
				if (i)
					std::cout << "hit " << hit++ << std::endl;
			}
			++i;
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
}

void Game::process_input(float dt)
{
	Inputs *i = inp;
	for (int j = 0; j < i->keyboard_count(); ++j) {
		// translation
		if (i->get_key(j, 17))
			players[j].move(0, .5, dt);
		if (i->get_key(j, 30))
			players[j].move(-.5, 0, dt);
		if (i->get_key(j, 32))
			players[j].move(.5, 0, dt);
		if (i->get_key(j, 31))
			players[j].move(0, -.5, dt);

		// rotation
		if (i->get_key(j, 105))
			players[j].rotate(3, dt);
		if (i->get_key(j, 106))
			players[j].rotate(-3, dt);

		// space
		if (i->get_key(j, 57)) {
			if (!prev_space[j] || space_counter[j] == 05) {
				createBullet(j);
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

void createBullet(int id)
{
	// Upwards is 0 rad, but in reality it's pi/2 rad
	// r = size / 2 / screenwidth
	// offset = center of player
	float r = 75.0 / 2.0 / 800.0;
	float x = cos(players[id].angle+PI/2.0) * r + (players[id].x + r);
	float y = sin(players[id].angle+PI/2.0) * r + (players[id].y - r);
	bullets.push_back(Bullet(x, y, players[id].angle));
	bullets[bullets.size() - 1].create_shader();
}
