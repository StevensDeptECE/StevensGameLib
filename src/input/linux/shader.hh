#ifndef _SHADER_H
#define _SHADER_H

class Shader {
private:
	const char *vertexShaderSource;
	const char *fragmentShaderSource;

	unsigned int vertexShader;
	unsigned int fragmentShader;

	unsigned int vLength;
	unsigned int iLength;
	unsigned int *indices;
	float *vertices;

public:
	unsigned int VBO, VAO;
	unsigned int EBO;
	unsigned int id;

	Shader();
	Shader(const char * const& vs, const char * const& fs);
	~Shader();
	void use();
	void bind(float *vert, int vlen, unsigned int *ind, int ilen);

};

#endif

