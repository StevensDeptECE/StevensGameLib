#if _MSC_VER >= 1600  /* 1600 is Microsoft Visual Studio 2010 */
#pragma execution_character_set("utf-8")
#endif

// Std
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <codecvt>
#include <locale>
// GLAD
#include <glad/glad.h> 
// GLFW
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#if 0
// TODO: add Freetype later
#include <ft2build.h>
#include FT_FREETYPE_H  
#endif

class GLWin {
private:
	GLFWwindow* win;
	uint32_t bgColor, fgColor;
	
	static void resize(GLFWwindow* window, int width, int height);
	void processInput(GLFWwindow *window);
	GLuint width, height; // the current height and width of this window
	GLuint VAO, VBO; // TODO: used for storing id of vertex array object and vertex buffer object. Should not be here
	// should be one per object that needs local storage on the graphics card, not one for the entire app
	glm::mat4 projection; // current projection for this window
	void fontInit();
public:
	GLWin(uint32_t width, uint32_t height, uint32_t bgColor, uint32_t fgColor, const char title[]);
	virtual void init() {}
	virtual void render() {}
	void mainLoop();
};

