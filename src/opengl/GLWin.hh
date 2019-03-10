// GLM
#include <glm/gtc/matrix_transform.hpp>

class GLFWwindow; // forward declaration, simplify: include file not needed here

class GLWin {
private:
	GLFWwindow* win;
	uint32_t bgColor, fgColor;
	
	static void resize(GLFWwindow* win, int width, int height);
	void processInput(GLFWwindow *win);
	uint32_t width, height; // the current height and width of this window
	glm::mat4 projection; // current projection for this window
	void fontInit();
public:
	GLWin(uint32_t width, uint32_t height, uint32_t bgColor, uint32_t fgColor, const char title[]);
	virtual void init() {}
	virtual void render() {}

	/* Manage animation with calls. Setting a time t, every time a tick happens
		 all animation moves forward to the next time t (integer)
	*/
	void resetAnim();      // set t=0
	void setTime(float t); // set t to any desired value
	void tick();           // move t forward
	void setEndTime(float t); // define the end time. When end is reached restart
	void setDesiredColor(const glm::vec3& c, float delta);

	// void setAnimation(Shape& s, glm::mat&) // transform s by matrix every tick

	void random(glm::vec3& v); // set this vec3 to random (0..1) for each
	/*
		void loadObject(Model& m, const char filename[]);
    void saveImage(Image& m); // save screenshot
		Shape* pick(int x, int y); // click on (x,y) and get back frontmost shape
		Shape* pick(int x, int y, Shape*); // click on (x,y), get Shape behind

	 */
	void mainLoop();
};

