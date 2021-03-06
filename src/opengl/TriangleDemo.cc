#include "GLWin.hh"
#include "shader.hpp"
using namespace std;

glm::vec3 operator *(const glm::vec3& v, double s) {
	return glm::vec3(v.x*s, v.y*s, v.z*s);
}
double dist(const glm::vec3& a, const glm::vec3& b) {
	double dx = a.x-b.x, dy = a.y-b.y, dz = a.z-b.z;
	return dx*dx + dy*dy + dz*dy;
}

class Triangle : public GLWin {
private:
	uint32_t vao, vbo;
	Shader s;
	glm::vec3 currentColor, targetColor, deltaColor;
public:
	Triangle() : GLWin(1024,800, 0xFF0000, 0xFFFFFF, "triangle"),
							 s("shaders/common.vs", "shaders/common.fs"),
							 currentColor(1,0,0)
	{
		targetColor = glm::vec3(0,0,0);
		deltaColor = (targetColor - currentColor) * 0.01;
	}
	~Triangle() {
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}
	void init() {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		static const GLfloat vertexBufferData[] =
			{ 
			 -1.0f, -1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			 0.0f,  1.0f, 0.0f,
			};
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER,
								 sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

	}
	void render() {
		s.use(); // invoke the shaders we defined here
		s.setVec3("ourColor", currentColor); // pass the color to the shader
		currentColor += deltaColor;
		if (dist(currentColor, targetColor) < 0.01) {
			random(targetColor);
			deltaColor = (targetColor - currentColor) * 0.01;
		}
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(
			0,  // attribute 0.  must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
		glDisableVertexAttribArray(0);
	}

};

int main() {
	try {
		Triangle t;
	
		t.mainLoop();
	} catch(const char* msg) {
		cerr << msg << '\n';
	}
}
		
