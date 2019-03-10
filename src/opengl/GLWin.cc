#if _MSC_VER >= 1600  /* 1600 is Microsoft Visual Studio 2010 */
#pragma execution_character_set("utf-8")
#endif

#include "shader.hpp"
#include "GLWin.hh"

// Std
#include <iostream>
#include <map>
#include <string>
#include <vector>
//#include <codecvt>
//#include <locale>
#include <random>

// GLFW
#include <GLFW/glfw3.h>

//matrix projections
//#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>

#if 0
// TODO: add Freetype later
#include <ft2build.h>
#include FT_FREETYPE_H  
#endif

using namespace std;

void GLWin::fontInit() {
	#if 0
	// Freetype
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	FT_Face face;
	// if (FT_New_Face(ft, "fonts/SourceHanSerifSC/SourceHanSerifSC-Regular.otf", 0, &face))
	// 	std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  

    if (FT_New_Face(ft, "./fonts/SourceHanSerifSC/SourceHanSerifSC-Regular.otf", 0, &face))
	    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  

	Shader textShader("shaders/text.vs", "shaders/text.fs");
	textShader.use();
	textShader.setMat4("projection", projection);
    FT_Set_Pixel_Sizes(face, 0, 48);
#endif
}

static default_random_engine gen;
static uniform_real_distribution<double> u01(0.0,1.0);

GLWin::GLWin(uint32_t width, uint32_t height, uint32_t bgColor, uint32_t fgColor, const char title[])
	: width(width), height(height), bgColor(bgColor), fgColor(fgColor) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
	win = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (win == nullptr){
		glfwTerminate();
		throw "Failed to open GLFW window";
	}
	glfwMakeContextCurrent(win);
	//	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw "Failed to initialize GLAD";
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	fontInit();
	projection = glm::ortho(0.0f, static_cast<GLfloat>(width), 0.0f, static_cast<GLfloat>(height));
	init();
}

void GLWin::mainLoop() {
	cout << sizeof(glm::mat4) << '\n';
	init(); // call the child class method to set up
	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents(); // Check and call events
		//TODO: replace by calling our general, portable multi-input event system
		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		render();
		glfwSwapBuffers(win);        // Swap buffer so the scene shows on screen
	}
	glfwTerminate();
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void GLWin::resize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void GLWin::random(glm::vec3& v) {
	v.x = u01(gen), v.y = u01(gen), v.y = u01(gen);
}
