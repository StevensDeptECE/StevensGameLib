#include "game.hh"
#include <ctime>
#include <iostream>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/*** Constants ***/


/*** Functions ***/
void framebuffer_size_callback(GLFWwindow *window, int width, int height);


int main(int argc, char **argv)
{
	Game game(1920, 1080, 0.3);
	std::srand(time(NULL));

	// GLFW Init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(Game::Width, Game::Height, "Asteroids Demo - OpenGL", glfwGetPrimaryMonitor(), NULL);
	if (window == nullptr) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed init GLAD" << std::endl;
		return -1;
	}

	game.init();

	GLfloat dt = 0.0f;
	GLfloat lastFrame = 0.0f;
	// Rendering loop
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		GLfloat currentFrame = glfwGetTime();
		dt = currentFrame - lastFrame;
		lastFrame = currentFrame;

		game.process_input(dt);
		game.update(dt);

		game.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

