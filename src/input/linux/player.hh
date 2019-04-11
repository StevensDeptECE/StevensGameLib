#pragma once

#include "game_object.hh"
#include "shader.hh"

class Player : public GameObject {
private:
	const static int num_indices;
	const static int num_vertices;

	void make_shader();
	void set_shape();

	Shader *shader;

public:

	Player(float a, float b);
//	Player(const Player& player);
//	~Player();	
	void move(float x, float y);
	void rotate(float angle);
	void set_transform();
	void create_shader();
	void update(float dt);
	void render() const;
	int should_remove() { return remove; }
	void do_remove() { remove = 1; }
};

