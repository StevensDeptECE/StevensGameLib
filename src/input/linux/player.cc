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
	size = 75.0f / 2.0;

	// Value between 0.4 and 1.0
	r = (float)((rand()%(10-4+1) + 4) / 10.0);
	g = (float)((rand()%(10-4+1) + 4) / 10.0);
	b = (float)((rand()%(10-4+1) + 4) / 10.0);
	b = 0;

	// Center player
	x = 0;
	y = 0;

	angle = 0.0f;
	set_shape();

	// Move player back
	x = a;
	y = b;
}

void Player::move(float x, float y)
{
	this->x += x;
	this->y += y * Game::Width / Game::Height;
}

void Player::rotate(float angle)
{
	this->angle += angle;
}

void Player::update(float dt)
{
	// Check boundaries
	if (x < -1)
		x = 1;
	if (x > 1)
		x = -1;
	if (y < -1)
		y = 1;
	if (y > 1)
		y = -1;

	set_transform();
}

void Player::set_shape()
{
	indices = new unsigned int[num_indices];
	vertices = new float[num_vertices];

	vertices[0] = x;
	vertices[1] = y + (float)size / Game::Height / ((float)Game::Width/Game::Height);
	vertices[2] = 0.0f;
	vertices[3] = x - (float)size / Game::Width;
	vertices[4] = y - (float)size / Game::Height / ((float)Game::Width/Game::Height);
	vertices[5] = 0.0f;
	vertices[6] = x;
	vertices[7] = y - (float)size / Game::Height / 2 / ((float)Game::Width/Game::Height);
	vertices[8] = 0.0f;
	vertices[9] = x + (float)size / Game::Width;
	vertices[10] = y - (float)size / Game::Height / ((float)Game::Width/Game::Height);
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
	shader->use();
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(x, y, 1.0));
	trans = glm::scale(trans, glm::vec3(1.0, (float)Game::Width/Game::Height, 1.0));
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
