#pragma once

class Shape {
private:
	GLuint vao, vbo;
	uint32_t numPoints;
public:
	void init(GLfloat vertices[], uint32_t numPoints) {
		this->numPoints = numPoints;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, len, vertices, GL_STATIC_DRAW);
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
		glDrawArrays(GL_TRIANGLES, 0, numPoints); // starting at 0 -> numpoints triangles
		glDisableVertexAttribArray(0);
	}
};
