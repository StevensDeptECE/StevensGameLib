#include "dbkbd.hh"
#include <time.h>
#include <pthread.h>
#include <cstring>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <vector>
#include <iostream>
#include <unistd.h>

using namespace std;

const char *vertexShaderTemp = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform mat4 transform;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderTemp = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(%ff, %ff, %ff, 1.0f);\n"
    "}\n\0";

class Player {
private:
	const int size = 75;
	double r, g, b;

	void make_shader()
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

public:
	unsigned int shaderProgram;
	unsigned int VBO, VAO;
	unsigned int EBO;
	
	unsigned int indices[6];


	// shared by all objects
	float x, y;
	float angle;
	float vertices[12] = {0};
	int length = 12;

	Player(float a, float b)// : x(a), y(b)
	{
		r = (double)rand() / RAND_MAX;
		g = (double)rand() / RAND_MAX;
		b = (double)rand() / RAND_MAX;
		length = 12;
		x = 0 - size / 800.0 / 2;
		y = 0 + size / 800.0 / 2;
		set_shape();
		x = a;
		y = b;
	}

	void move(float x, float y)
	{
		this->x += x;
		this->y += y;
	}

	void rotate(float angle)
	{
		this->angle += angle;
	}

	void set_shape()
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

	void set_transform()
	{
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(x+size/800.0/2, y-size/800.0/2, 1.0));
		trans = glm::rotate(trans, angle, glm::vec3(0.0, 0.0, 1.0));

		unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	}


	void create_shader()
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
};

vector<Player> players;
bool play = true;

void *my_keyboard_handler(void *);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
int main(int argc, char **argv)
{
	srand(time(NULL));
	Inputs inp;

	// Create players
	int num_players = inp.keyboard_count(); 
	cout << "Keyboards: " << num_players << endl;

	for (int j = 0; j < num_players; ++j)
		players.push_back(Player((double)rand() / RAND_MAX - .1, (double)rand() / RAND_MAX - .1));

	// Create keyboard handler thread
	pthread_t myThread;
	pthread_create(&myThread, NULL, &my_keyboard_handler, &inp);

	// GLFW Init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 800, "Keyboard Demo - OpenGL", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed init GLAD" << endl;
		return -1;
	}

	// Do all the OpenGL stuff with vao, vbo, and ebo
	for (int j = 0; j < num_players; ++j) {
		players[j].create_shader();
	}


	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw square per player
		for (int j = 0; j < num_players; ++j) {
			glUseProgram(players[j].shaderProgram);
			players[j].set_transform();
			glBindVertexArray(players[j].VAO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, players[j].EBO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	for (int j = 0; j < num_players; ++j) {
		glDeleteVertexArrays(1, &players[j].VAO);
		glDeleteBuffers(1, &players[j].VBO);
		glDeleteBuffers(1, &players[j].EBO);
	}

	glfwTerminate();

	// TODO: Figure out how to kill thread immediately
	play = false;

	return 0;
}

void *my_keyboard_handler(void *input)
{
	Inputs *i = static_cast<Inputs *>(input);
	while (play) {
		for (int j = 0; j < i->keyboard_count(); ++j) {
			float o_x = players[j].x;
			float o_y = players[j].y;
			if (i->get_key(j, 17))
				players[j].move(0, .0005);
			if (i->get_key(j, 30))
				players[j].move(-.0005, 0);
			if (i->get_key(j, 32))
				players[j].move(.0005, 0);
			if (i->get_key(j, 31))
				players[j].move(0, -.0005);

			// rotation
			if (i->get_key(j, 105))
				players[j].rotate(.005);
			if (i->get_key(j, 106))
				players[j].rotate(-.005);
		}
		usleep(1000);	// sample at 2x framerate?
	}
	return nullptr;
}
