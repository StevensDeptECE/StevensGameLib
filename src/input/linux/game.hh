#ifndef _GAME_H
#define _GAME_H

#include "game_object.hh"
#include "player.hh"
#include "bullet.hh"
#include "asteroid.hh"

// Represents the current state of the game
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

class Game
{
	public:
		GameState  State;	
		static int Width, Height;
		Game(int width, int height);
		~Game();
		void init();
		void process_input(float dt);
		void update(float dt);
		void render();
		int check_collision(GameObject &obj_a, GameObject &obj_b);
		void physics(float dt);
};

#endif
