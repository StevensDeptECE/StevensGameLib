#include "bullet.hh"
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

unsigned int Bullet::shaderProgram = -1;

void Bullet::make_shader()
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

	std::cout << "made shader for bullet" << std::endl;
}


Bullet::Bullet(float x, float y, float c)
{
	this->x = x;
	this->y = y;
	r = 1.0f;
	g = 1.0f;
	b = 0.0f;
	length = 12;
	set_shape();
	angle = 0.0f;
	rotate(c);
	remove = 0;
}

void Bullet::move(float x, float y)
{
	this->x += x;
	this->y += y;
}

void Bullet::rotate(float angle)
{
	this->angle += angle + glm::pi<float>()/2;
}

void Bullet::update()
{
	dist += 0.001;	
	
	// Check boundaries
	remove = (x+dist*cos(angle) < -1 || x+dist*cos(angle) > 1
			|| y+dist*sin(angle) < -1 || y+dist*sin(angle) > 1);
}

int Bullet::should_remove()
{
	return remove;
}

void Bullet::set_shape()
{
	int steps = 30;
	float start_angle = 0.0f;
	float end_angle = 2 * glm::pi<float>();

	float t = start_angle;

	float radius_outer = 10.0f / 800.0;

	int pos = 0;

	for (int i = 0; i <= steps; i++) {
		float x_inner = 0;
		float y_inner = 0;

		float x_outer = radius_outer * cos(t) + 0;
		float y_outer = radius_outer * sin(t) + 0;

		vertices[pos++] = x_inner;
		vertices[pos++] = y_inner;
		vertices[pos++] = 0.0f;

		vertices[pos++] = x_outer;
		vertices[pos++] = y_outer;
		vertices[pos++] = 0.0f;

		t += (end_angle - start_angle) / steps;
	}
}

void Bullet::set_transform()
{
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(x + dist * cos(angle), y + dist * sin(angle), 1.0));

	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
}

void Bullet::create_shader()
{
	if (shaderProgram == -1) {
		shaderProgram = glCreateProgram();
		make_shader();
	}

	// 1. bind Vertex Array Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// 2. copy vertices array into Vertex Buffer Object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	// 3. copy indices array in element buffer
//	glGenBuffers(1, &EBO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. set vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Bullet::render()
{
	glUseProgram(shaderProgram);
	set_transform();
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 31 * 2);
	glDisableVertexAttribArray(0);
}
