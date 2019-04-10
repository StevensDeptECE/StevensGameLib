#ifndef _PLAYER_H
#define _PLAYER_H

#include "game_object.hh"
#include "shader.hh"

class Player : public GameObject {
private:
	const static int num_indices;
	const static int num_vertices;

	void make_shader();
	void set_shape();
	void set_transform();

	Shader *shader;

public:

	Player(float a, float b);
//	Player(const Player& player);
//	~Player();	
	void move(float x, float y);
	void rotate(float angle);
	void create_shader();
	void update(float dt);
	void render();
	int should_remove() { return remove; }
	void do_remove() { remove = 1; }
};

#endif
