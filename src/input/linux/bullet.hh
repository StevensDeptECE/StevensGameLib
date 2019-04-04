#ifndef _BULLET_H
#define _BULLET_H

#include "shader.hh"

class Bullet {
private:
	double r, g, b;
	unsigned int indices[6];
	float x, y;
	float angle;
	float dist = 0;
	float vertices[31 * 6] = {0};
	int remove;

	void make_shader();
	void set_shape();
	void set_transform();

	static Shader *shader;

public:
	static unsigned int shaderDone;

	Bullet(float a, float b, float c);
	void move(float x, float y);
	void rotate(float angle);
	void create_shader();
	void update(float dt);
	void render();
	int should_remove();
};

#endif
