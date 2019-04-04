#include "shader.hh"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Shader::Shader() {}

Shader::Shader(const char * const& vs, const char * const& fs)
{
	id = glCreateProgram();

	vertexShaderSource = vs;
	fragmentShaderSource = fs;

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

// Needs to be more generic
void Shader::bind(float *vert, int vlen, unsigned int *ind, int ilen)
{
	// 1. bind Vertex Array Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// 2. copy vertices array into Vertex Buffer Object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vlen*sizeof(float), vert, GL_DYNAMIC_DRAW);

	// 3. copy indices array in element buffer
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ilen*sizeof(float), ind, GL_STATIC_DRAW);

	// 4. set vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Shader::use()
{
	glUseProgram(id);
}
