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
        // Game state
        GameState  State;	
        static int Width, Height;
        // Constructor/Destructor
        Game(int width, int height);
        ~Game();
        // Initialize game state (load all shaders/textures/levels)
        void init();
        // GameLoop
        void process_input(float dt);
        void update(float dt);
        void render();
		int check_collision(Asteroid obj_a, Bullet obj_b);
		int check_collision(Asteroid obj_a, Player obj_b);
		void physics(float dt);
};

#endif
