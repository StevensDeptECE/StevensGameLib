#ifndef _BULLET_H
#define _BULLET_H

#include "shader.hh"

class Bullet {
private:
	double r, g, b;
	unsigned int indices[6];
	float vertices[31 * 6] = {0};

	void make_shader();
	void set_shape();
	void set_transform();

	static Shader *shader;

public:
	static unsigned int shaderDone;

	float dist = 0;
	float x, y;
	float angle;
	int remove;

	Bullet(float a, float b, float c);
	void move(float x, float y);
	void rotate(float angle);
	void create_shader();
	void update(float dt);
	void render();
	int should_remove();
};

#endif
