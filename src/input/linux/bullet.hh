#ifndef _BULLET_H
#define _BULLET_H

#include "game_object.hh"
#include "shader.hh"

class Bullet : public GameObject {
private:
	const static int num_indices;
	const static int num_vertices;

	void make_shader();
	void set_shape();
	void set_transform();

	static Shader *shader;

public:
	static unsigned int shaderDone;

	Bullet(float x, float y, float a);
//	Bullet(const Bullet& bul);
//	~Bullet();
	void move(float x, float y) {}
	void rotate(float angle);
	void create_shader();
	void update(float dt);
	void render() const;
	int should_remove() { return remove; }
	void do_remove() { remove = 1; }
};

#endif
