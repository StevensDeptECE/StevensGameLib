#ifndef _PLAYER_H
#define _PLAYER_H

class Player {
private:
	const int size = 75;
	double r, g, b;
	unsigned int indices[6];
	float x, y;
	float angle;
	float vertices[12] = {0};
	int length = 12;

	void make_shader();
	void set_shape();
	void set_transform();

public:
	unsigned int shaderProgram;
	unsigned int VBO, VAO;
	unsigned int EBO;

	Player(float a, float b);
	void move(float x, float y);
	void rotate(float angle);
	void create_shader();
	void render();
};

#endif
