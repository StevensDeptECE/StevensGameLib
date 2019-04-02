#ifndef _BULLET_H
#define _BULLET_H

class Bullet {
private:
	double r, g, b;
	unsigned int indices[6];
	float x, y;
	float angle;
	int length = 12;
	float dist = 0;
	float vertices[31 * 6] = {0};
	int remove;

	void make_shader();
	void set_shape();
	void set_transform();

public:
	static unsigned int shaderProgram;
	unsigned int VBO, VAO;
	unsigned int EBO;

	Bullet(float a, float b, float c);
	void move(float x, float y);
	void rotate(float angle);
	void create_shader();
	void update();
	void render();
	int should_remove();
};

#endif
