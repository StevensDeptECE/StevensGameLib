#pragma once

#include "game_object.hh"
#include "player.hh"
#include "bullet.hh"
#include "asteroid.hh"
#include "dbkbd.hh"

#include <vector>

// Represents the current state of the game
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

enum InputTypes {
	QUIT = 0,
	NEWGAME = 1,
	NUCLEAR_OPTION = 3,
	SELF_DESTRUCT = 2
};



class Game {
private:
	Inputs *inp;

	float accumulator; // count time to create_asteroid()
	float asteroid_creation_time;
	/*** Entities ***/
	std::vector<Player> players;
	std::vector<Bullet> bullets;
	std::vector<Asteroid> asteroids;

	/*** Control ***/
	// Put this stuff in a struct
	// or something useful
	int *space_counter; // how many spaces typed
	int *prev_space; // 
	int *type;
	int *num_bullets;

	int score;
	int deaths;

	void create_bullet(int id);
	void create_asteroid();

public:
	GameState  State;	
	static int Width, Height;
	Game(int width, int height, float ast = 0.3f);
	~Game();
	void init();
	void process_input(float dt);
	void update(float dt);
	void render();
	bool check_collision(const GameObject &a, const GameObject &b) const;
	void physics(float dt);
};

