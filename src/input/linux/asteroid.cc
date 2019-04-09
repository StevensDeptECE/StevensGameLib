#include "asteroid.hh"
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

unsigned int Asteroid::shaderDone = 0;

Shader *Asteroid::shader;

void Asteroid::make_shader()
{
	char *vertexShaderSource = (char*)malloc(256);
	char *fragmentShaderSource = (char*)malloc(256);
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	strcpy(vertexShaderSource, vertexShaderTemp);
	sprintf(fragmentShaderSource, fragmentShaderTemp, r, g, b);

	shader = new Shader(vertexShaderSource, fragmentShaderSource);
	std::cout << "made shader for asteroid" << std::endl;
}


Asteroid::Asteroid(float x, float y, float a)
{
	this->x = x;
	this->y = y;
	r = 1.0f;
	g = 0.7f;
	b = 0.5f;
	set_shape();
	angle = 0;
	rotate(a);
	remove = 0;
}

void Asteroid::rotate(float angle)
{
	this->angle += angle + glm::pi<float>()/2;
}

void Asteroid::update(float dt)
{

	x += 1.2 * dt * cos(angle);
	y += 1.2 * dt * sin(angle);

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
}

int Asteroid::should_remove()
{
	return remove;
}

void Asteroid::set_shape()
{
	int steps = 30;
	float start_angle = 0.0f;
	float end_angle = 2 * glm::pi<float>();

	float t = start_angle;

	float radius_outer = size / 800.0;

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

void Asteroid::set_transform()
{
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(x, y, 1.0));
	trans = glm::rotate(trans, angle, glm::vec3(0.0, 0.0, 1.0));

	unsigned int transformLoc = glGetUniformLocation(shader->id, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
}

void Asteroid::create_shader()
{
	if (!shaderDone) {
		make_shader();
		shaderDone = 1;
	}

	shader->bind(vertices, 31 * 6, indices, 6);
}

void Asteroid::render()
{
	shader->use();
	set_transform();
	glBindVertexArray(shader->VAO);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 31 * 2);
	glDisableVertexAttribArray(0);
}
