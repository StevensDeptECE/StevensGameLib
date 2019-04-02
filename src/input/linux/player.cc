#include "player.hh"
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


void Player::make_shader()
{
	char *vertexShaderSource = (char*)malloc(256);
	char *fragmentShaderSource = (char*)malloc(256);
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	strcpy(vertexShaderSource, vertexShaderTemp);
	sprintf(fragmentShaderSource, fragmentShaderTemp, r, g, b);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	free(vertexShaderSource);
	free(fragmentShaderSource);
}


Player::Player(float a, float b)
{
	r = (double)rand() / RAND_MAX * .25 + .75;
	g = (double)rand() / RAND_MAX * .25 + .75;
	b = (double)rand() / RAND_MAX * .25 + .75;
	length = 12;
	x = 0 - size / 800.0 / 2;
	y = 0 + size / 800.0 / 2;
	set_shape();
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

void Player::update()
{
	// Check boundaries
	float cx = x+size/2.0/800.0;
	float cy = y-size/2.0/800.0;
	//std::cout << "cx: " << cx << "  cy: " << cy << std::endl;
	if (cx < -1)
		x = 1-size/2.0/800.0;
	if (cx > 1)
		x = -1;
	if (cy < -1)
		y = 1;
	if (cy > 1)
		y = -1+size/2.0/800.0;
	//trans = glm::translate(trans, glm::vec3(x+size/800.0/2, y-size/800.0/2, 1.0));

}

void Player::set_shape()
{
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
	trans = glm::translate(trans, glm::vec3(x+size/800.0/2, y-size/800.0/2, 1.0));
	trans = glm::rotate(trans, angle, glm::vec3(0.0, 0.0, 1.0));

	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
}

void Player::create_shader()
{
	shaderProgram = glCreateProgram();
	make_shader();


	// 1. bind Vertex Array Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// 2. copy vertices array into Vertex Buffer Object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	// 3. copy indices array in element buffer
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. set vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Player::render()
{
	glUseProgram(shaderProgram);
	set_transform();
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
}
