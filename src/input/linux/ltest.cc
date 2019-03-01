#include "dbkbd.hh"
#include <time.h>
#include <pthread.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
const char *fragmentShader2Source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.2f, 0.5f, 1.0f, 1.0f);\n"
    "}\n\0";


class Player {
private:
	const int size = 125;
	double r, g, b;

public:
	float x, y;
	float vertices[12] = {0};
	int length = 12;
	Player(float a, float b) : x(a), y(b)
	{
		r = (double)rand() / RAND_MAX;
		g = (double)rand() / RAND_MAX;
		b = (double)rand() / RAND_MAX;
		vertices[0] = x;
		vertices[1] = y;
		vertices[2] = 0.0f;
		vertices[3] = x+size/800.0;
		vertices[4] = y;
		vertices[5] = 0.0f;
		vertices[6] = x+size/800.0;
		vertices[7] = y+size/600.0;
		vertices[8] = 0.0f;
		vertices[9] = x;
		vertices[10] = y+size/600.0;
		vertices[11] = 0.0f;
		length = 12;
	}

	void move(float x, float y)
	{
		this->x += x;
		this->y += y;
		update();
	}

	void update()
	{
		vertices[0] = x;
		vertices[1] = y;
		vertices[2] = 0.0f;
		vertices[3] = x+size/800.0;
		vertices[4] = y;
		vertices[5] = 0.0f;
		vertices[6] = x+size/800.0;
		vertices[7] = y+size/600.0;
		vertices[8] = 0.0f;
		vertices[9] = x;
		vertices[10] = y+size/600.0;
		vertices[11] = 0.0f;
	}
};

vector<Player> players;
bool isIdle = true;
bool play = true;
//void draw()
//{
//	glClearColor(0.4, 0.4, 0.4, 0.4);
//	glClear(GL_COLOR_BUFFER_BIT);
//	for (int i = 0; i < players.size(); ++i)
//		players[i].draw();
//}

void idle()
{
//	if (!isIdle)
//		glutPostRedisplay();
//	isIdle = true;
	usleep(10000);
}

void *my_keyboard_handler(void *);
void drawTriangle();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
int main(int argc, char **argv)
{
	srand(time(NULL));
	Inputs i;

	cout << "Keyboards: " << i.keyboard_count() << endl;

	pthread_t myThread;
	pthread_create(&myThread, NULL, &my_keyboard_handler, &i);

	for (int j = 0; j < i.keyboard_count(); ++j)
		players.push_back(Player((double)rand() / RAND_MAX - .1, (double)rand() / RAND_MAX - .1));

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "Keyboard Demo - OpenGL", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed init GLAD" << endl;
		return -1;
	}

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	unsigned int shaderProgram = glCreateProgram();
	unsigned int shaderProgram2 = glCreateProgram();

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glShaderSource(fragmentShader2, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader2);
	// 1
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// 2
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram);


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};


	//glViewport(0, 600, 800, 0);

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*players[0].length, players[0].vertices, GL_DYNAMIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		for (int j = 0; j < i.keyboard_count(); ++j) {
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*players[j].length, players[j].vertices, GL_DYNAMIC_DRAW);
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();

	play = false;

	return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, height, width, 0);
}

void *my_keyboard_handler(void *input)
{
	Inputs *i = static_cast<Inputs *>(input);
	while (play) {
		for (int j = 0; play && j < players.size(); ++j) {
			float o_x = players[j].x;
			float o_y = players[j].y;
			if (i->get_key(j, 103))
				players[j].move(0, .005);
			if (i->get_key(j, 105))
				players[j].move(-.005, 0);
			if (i->get_key(j, 106))
				players[j].move(.005, 0);
			if (i->get_key(j, 108))
				players[j].move(0, -.005);
			if (o_x != players[j].x || o_y != players[j].y)
				isIdle = false;
		}
		usleep(5000);	// sample at 2x framerate?
	}
	return nullptr;
}
