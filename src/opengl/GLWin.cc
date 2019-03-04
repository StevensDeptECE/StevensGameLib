#include "GLWin.hh"
using namespace std;

GLWin::fontInit() {
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

GLWin::GLWin(uint32_t width, uint32_t height, uint32_t bgColor, uint32_t fgColor, const char title[])
	: width(width), height(height), bgColor(bgColor), fgColor(fgColor) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
	win = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == nullptr){
		glfwTerminate();
		throw "Failed to open GLFW window";
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw "Failed to initialize GLAD";
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	fontInit();
	projection = glm::ortho(0.0f, static_cast<GLfloat>(WIDTH), 0.0f, static_cast<GLfloat>(HEIGHT));
}

GLWin::mainLoop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents(); // Check and call events
		//TODO: replace by calling our general, portable multi-input event system
		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		render();
		glfwSwapBuffers(window);        // Swap buffer so the scene shows on screen
	}
	glfwTerminate();
}