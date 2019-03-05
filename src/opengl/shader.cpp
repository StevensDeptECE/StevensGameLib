#include "shader.hpp"
#include <sstream>
#include <fstream>
using namespace std;

uint32_t Shader::load(const char filename[], GLuint shaderType, const char shaderTypeName[]) {
	ifstream shaderFile(filename);
	if (!shaderFile)
		throw "Failed to load shader";
	stringstream shaderStream;
	shaderStream << shaderFile.rdbuf();
	shaderFile.close();
	string shaderCode = shaderStream.str();
	uint32_t id = glCreateShader(shaderType);
	const char* sourceCode = shaderCode.c_str();
	glShaderSource(id, 1, &sourceCode, nullptr);
	glCompileShader(id);
	checkCompileErrors(id, shaderTypeName);
	return id;
}

void Shader::checkCompileErrors(GLuint shader, const std::string& type) {
	GLint success;
	GLchar infoLog[1024];
	if(type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- --\n";
		}
	} else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if(!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- --\n";
		}
	}
}

Shader::Shader(const char vertexPath[], const char fragmentPath[], const char geometryPath[]) {
	try {
		uint32_t vertexShaderID = load(vertexPath, GL_VERTEX_SHADER, "VERTEX");          // load the vertex shader
		uint32_t fragmentShaderID = load(fragmentPath, GL_FRAGMENT_SHADER, "FRAGMENT");  // load the fragment shader
		uint32_t geometryShaderID;
		uint32_t progID = glCreateProgram();
		glAttachShader(progID, vertexShaderID);
		glAttachShader(progID, fragmentShaderID);
		// if geometry shader path is present, also load a geometry shader
		if (geometryPath != nullptr) {
			geometryShaderID = load(geometryPath, GL_GEOMETRY_SHADER, "GEOMETRY");
			glAttachShader(progID, geometryShaderID);
		}
		glLinkProgram(progID);
		checkCompileErrors(progID, "PROGRAM");
		// delete the shaders as they're linked in now and no longer necessery
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		if(geometryPath != nullptr)
			glDeleteShader(geometryShaderID);
	} catch (const ifstream::failure& e) {
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
	}
}
