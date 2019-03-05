#pragma once

/*
 * Shader encapsulates the ugly GL API for compiling shaders.
 * the private method load() loads shaders of vertex, fragment or geometry types
 * The constructor links these together in a program and saves only the resulting linked ID.
 * This object throws out all scratch data except for the program ID and what is stored on the graphics card.
 * Author: Dov Kruger March 5, 2019
 * TODO: methods are passed string objects which is a bit wasteful if they are constants. We should switch to const char*
 * not that big a deal because it only happens when compiling and during rendering nothing happens.
 */
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <iostream>

class Shader {
private:
	uint32_t progID;
	uint32_t load(const char filename[], uint32_t shaderType, const char shaderTypeName[]);
public:
	Shader(const char vertexPath[], const char fragmentPath[], const char geometryPath[] = nullptr);

	void use() const { 
		glUseProgram(progID); 
	}

	void setBool(const std::string &name, bool value) const {         
		glUniform1i(glGetUniformLocation(progID, name.c_str()), (int)value); 
	}

	void setInt(const std::string &name, int value) const { 
		glUniform1i(glGetUniformLocation(progID, name.c_str()), value); 
	}

	void setFloat(const std::string &name, float value) const { 
		glUniform1f(glGetUniformLocation(progID, name.c_str()), value); 
	}

	void setVec2(const std::string &name, const glm::vec2 &value) const { 
		glUniform2fv(glGetUniformLocation(progID, name.c_str()), 1, &value[0]); 
	}

	void setVec2(const std::string &name, float x, float y) const { 
		glUniform2f(glGetUniformLocation(progID, name.c_str()), x, y); 
	}

	void setVec3(const std::string &name, const glm::vec3 &value) const { 
		glUniform3fv(glGetUniformLocation(progID, name.c_str()), 1, &value[0]); 
	}

	void setVec3(const std::string &name, float x, float y, float z) const { 
		glUniform3f(glGetUniformLocation(progID, name.c_str()), x, y, z); 
	}

	void setVec4(const std::string &name, const glm::vec4 &value) const { 
		glUniform4fv(glGetUniformLocation(progID, name.c_str()), 1, &value[0]); 
	}
	void setVec4(const std::string &name, float x, float y, float z, float w) { 
		glUniform4f(glGetUniformLocation(progID, name.c_str()), x, y, z, w); 
	}
    
	void setMat2(const std::string &name, const glm::mat2 &mat) const {
		glUniformMatrix2fv(glGetUniformLocation(progID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void setMat3(const std::string &name, const glm::mat3 &mat) const {
		glUniformMatrix3fv(glGetUniformLocation(progID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
    
	void setMat4(const std::string &name, const glm::mat4 &mat) const {
		glUniformMatrix4fv(glGetUniformLocation(progID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

};
