#ifndef _PLAYER_H
#define _PLAYER_H

#include "shader.hh"

class Player {
private:
	const int size = 75;
	double r, g, b;
	unsigned int indices[6] = {0};
	float vertices[12] = {0};
	int length = 12;

	void make_shader();
	void set_shape();
	void set_transform();

	Shader *shader;

public:
	float x, y;
	float angle;

	Player(float a, float b);
	void move(float x, float y, float dt);
	void rotate(float angle, float dt);
	void create_shader();
	void render();
	void update(float dt);
};

#endif
