#include "dbkbd.hh"
#include "player.hh"
#include "bullet.hh"

#include <ctime>
#include <pthread.h>
#include <cstring>
#include <vector>
#include <iostream>
#include <unistd.h>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


std::vector<Player> players;
std::vector<Bullet> bullets;
bool play = true;
int bulletId = -1;
int *bulletQueue;

void *handle_input(void *);
void handle_input();
void *update(void *);
void render();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void createBullet(int id);

Inputs *p_inp = nullptr;

int main(int argc, char **argv)
{
	std::srand(time(NULL));
	Inputs inp;
	p_inp = &inp;

	// Create players
	int num_players = inp.keyboard_count(); 
	std::cout << "Keyboards: " << num_players << std::endl;

	for (int j = 0; j < num_players; ++j)
		players.push_back(Player((double)rand() / RAND_MAX - .1, (double)rand() / RAND_MAX - .1));

	bulletQueue = new int[num_players];
	for (int i = 0; i < num_players; ++i)
		bulletQueue[i] = 0;

	// Create keyboard handler thread
	pthread_t myThread;
	pthread_create(&myThread, NULL, &update, (void *)NULL);

	// GLFW Init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 800, "Keyboard Demo - OpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed init GLAD" << std::endl;
		return -1;
	}

	// Do all the OpenGL stuff with vao, vbo, and ebo
	for (int j = 0; j < num_players; ++j) {
		players[j].create_shader();
	}

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		render();

		for (int i = 0; i < num_players; ++i) {
			if (bulletQueue[i]) {
				createBullet(i);
				bulletQueue[i] = 0;
			}
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


void *update(void *)
{
	while (play) {
		handle_input();
		for (int i = 0; i < bullets.size(); ++i) {
			//	players[i].update();
			bullets[i].update();
		}
		usleep(1000);
	}
	return nullptr;
}

void render() {
	// Draw square per player
	for (int j = 0; j < players.size(); ++j) {
		players[j].render();
	}

	for (int j = 0; j < bullets.size(); ++j) {
		bullets[j].render();
	}

}

const float pi = 3.141592654;
void createBullet(int id)
{
	
	float x = cos(players[id].angle+pi/2.0) * (75 / 800.0 / 2) + (players[id].x + 75 / 800.0 / 2);
	float y = sin(players[id].angle+pi/2.0) * (75 / 800.0 / 2) + (players[id].y - 75 / 800.0 / 2);
	bullets.push_back(Bullet(x, y, players[id].angle));
	bullets[bullets.size() - 1].create_shader();
}

int space_counter = 0;
int prev_space = 0;
void handle_input()
{
	Inputs *i = p_inp;
	for (int j = 0; j < i->keyboard_count(); ++j) {
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

		// space
		if (i->get_key(j, 57)) {
			if (!prev_space || space_counter == 100) {
				bulletQueue[j] = 1;
				space_counter = 0;
			}
			prev_space = 1;
			++space_counter;
		} else if (!i->get_key(j, 57)) {
			prev_space = 0;
			space_counter = 0;
		}

	}
}

void *handle_input(void *input)
{
	Inputs *i = static_cast<Inputs *>(input);
	while (play) {
		for (int j = 0; j < i->keyboard_count(); ++j) {
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
		usleep(1000);
	}
	return nullptr;
}
