#include "GLWin.hh"
using namespace std;
class Triangle : public GLWin {
private:
	GLuint vao, vbo;

public:
	Triangle() : GLWin(1024,800, 0xFF0000, 0xFFFFFF, "triangle") {

	}
	~Triangle() {
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
		glDeleteProgram(programID);
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

}
int main() {
	try {
		Triangle t;
	
		t.mainLoop();
	} catch(const char* msg) {
		cerr << msg << '\n';
	}
}
		
