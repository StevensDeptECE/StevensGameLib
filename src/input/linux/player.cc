#include "game.hh"
#include "player.hh"
#include "shader.hh"
#include <cstdio>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const static char *vertexShaderTemp = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform mat4 transform;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const static char *fragmentShaderTemp = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(%ff, %ff, %ff, 1.0f);\n"
    "}\n\0";

const int Player::num_indices = 6;
const int Player::num_vertices = 12;

void Player::make_shader()
{
	char *vertexShaderSource = new char[256];
	char *fragmentShaderSource = new char[256];
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	strcpy(vertexShaderSource, vertexShaderTemp);
	sprintf(fragmentShaderSource, fragmentShaderTemp, r, g, b);

	shader = new Shader(vertexShaderSource, fragmentShaderSource);
}


Player::Player(float a, float b)
{
	size = 75.0f;

	// Value between 0.4 and 1.0
	r = (float)((rand()%(10-4+1) + 4) / 10.0);
	g = (float)((rand()%(10-4+1) + 4) / 10.0);
	b = (float)((rand()%(10-4+1) + 4) / 10.0);

	// Center player
	x = 0 - size / Game::Width / 2;
	y = 0 + size / Game::Height / 2;

	angle = 0.0f;
	set_shape();

	// Move player back
	x = a;
	y = b;
}

void Player::move(float x, float y)
{
	this->x += x;
	this->y += y;
}

void Player::rotate(float angle)
{
	this->angle += angle;
}

void Player::update(float dt)
{
	// Check boundaries
	float cx = x+size/2.0/800.0;
	float cy = y-size/2.0/800.0;
	if (cx < -1)
		x = 1-size/2.0/800.0;
	if (cx > 1)
		x = -1;
	if (cy < -1)
		y = 1;
	if (cy > 1)
		y = -1+size/2.0/800.0;

	set_transform();
}

void Player::set_shape()
{
	indices = new unsigned int[num_indices];
	vertices = new float[num_vertices];

	if (this->x < -1)
		this->x = -1;
	if (this->x > 1-size/800.0)
		this->x = 1-size/800.0;
	if (this->y > 1)
		this->y = 1;
	if (this->y < size/800.0-1)
		this->y = size/800.0-1;
	vertices[0] = x + size / 800.0 / 2;
	vertices[1] = y;
	vertices[2] = 0.0f;
	vertices[3] = x;
	vertices[4] = y - size / 800.0;
	vertices[5] = 0.0f;
	vertices[6] = x+size/800.0 / 2;
	vertices[7] = y-size/800.0 / 1.5;
	vertices[8] = 0.0f;
	vertices[9] = x + size / 800.0;
	vertices[10] = y-size/800.0;
	vertices[11] = 0.0f;

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;
}

void Player::set_transform()
{
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(x+size/Game::Width/2.0, y-size/Game::Height/2.0, 1.0));
	trans = glm::rotate(trans, angle, glm::vec3(0.0, 0.0, 1.0));

	unsigned int transformLoc = glGetUniformLocation(shader->id, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
}

void Player::create_shader()
{
	make_shader();
	shader->bind(vertices, num_vertices, indices, num_indices);
}

void Player::render() const
{
	shader->use();
	glBindVertexArray(shader->VAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shader->EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
}
