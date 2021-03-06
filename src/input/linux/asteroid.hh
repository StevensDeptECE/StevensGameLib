#pragma once

#include "game_object.hh"
#include "shader.hh"

class Asteroid : public GameObject {
private:
	const static int num_indices;
	const static int num_vertices;

	void make_shader();
	void set_shape();

	Shader *shader;

public:
	static unsigned int shaderDone;

	Asteroid(float x, float y, float a, float s);
//	Asteroid(const Asteroid& ast);
//	~Asteroid();
	void move(float x, float y) {}
	void rotate(float angle);
	void create_shader();
	void update(float dt);
	void render() const;
	void set_transform();
	int should_remove() { return remove; }
	void do_remove() { remove = 1; }
};

