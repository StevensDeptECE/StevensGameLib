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
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

/*** Constants ***/
const float PI = glm::pi<float>();

/*** Entities ***/
std::vector<Player> players;
std::vector<Bullet> bullets;

/*** Control ***/
bool play = true;
int bulletId = -1;
int *bulletQueue;
// Put this stuff in a struct
// or something useful
int *space_counter;
int *prev_space;

/*** Functions ***/
void createBullet(int id);
void handle_input();
void *update(void *);
void render();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

Inputs *inp;

int main(int argc, char **argv)
{
	inp = new Inputs;
	std::srand(time(NULL));

	// Create players
	int num_players = inp->keyboard_count(); 
	std::cout << "Keyboards: " << num_players << std::endl;

	// Control arrays
	bulletQueue = new int[num_players];
	space_counter = new int[num_players];
	prev_space = new int[num_players];

	for (int j = 0; j < num_players; ++j) {
		players.push_back(Player((double)rand() / RAND_MAX - .1, (double)rand() / RAND_MAX - .1));
		bulletQueue[j] = 0;
		space_counter[j] = 0;
		prev_space[j] = 0;
	}

	// Update thread
	pthread_t myThread;
	pthread_create(&myThread, NULL, &update, (void *)NULL);

	// GLFW Init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 800, "Asteroids Demo - OpenGL", NULL, NULL);
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

	// Player shaders, VAOs, VBOs, EBOs, etc.
	for (int j = 0; j < num_players; ++j) {
		players[j].create_shader();
	}

	// Rendering loop
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		render();

		// By observation, objects must be created in main loop
		// Need to figure out why
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
		int i = 0;
		while (i < bullets.size()) {
			bullets[i].update();
			if (bullets[i].should_remove()) {
				bullets.erase(bullets.begin() + i);
			} else {
				++i;
			}
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

void createBullet(int id)
{
	// Upwards is 0 rad, but in reality it's pi/2 rad
	// r = size / 2 / screenwidth
	// offset = center of player
	float r = 75.0 / 2.0 / 800.0;
	float x = cos(players[id].angle+PI/2.0) * r + (players[id].x + r);
	float y = sin(players[id].angle+PI/2.0) * r + (players[id].y - r);
	bullets.push_back(Bullet(x, y, players[id].angle));
	bullets[bullets.size() - 1].create_shader();
}

void handle_input()
{
	Inputs *i = inp;
	for (int j = 0; j < i->keyboard_count(); ++j) {
		// translation
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
			if (!prev_space[j] || space_counter[j] == 100) {
				bulletQueue[j] = 1;
				space_counter[j] = 0;
			}
			prev_space[j] = 1;
			++space_counter[j];
		} else if (!i->get_key(j, 57)) {
			prev_space[j] = 0;
			space_counter[j] = 0;
		}
	}
}

