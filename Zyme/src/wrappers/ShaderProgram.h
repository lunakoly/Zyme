#pragma once
#include <string>
#include <GL\glew.h>
#include <glm\glm.hpp>


class ShaderProgram {
private:
	GLuint value;

public:
	ShaderProgram();
	~ShaderProgram();

	ShaderProgram * use();
	// SAME
	const ShaderProgram * use() const;
	
	ShaderProgram * setIntUniform(const std::string &, GLint);
	ShaderProgram * setFloatUniform(const std::string &, GLfloat);
	ShaderProgram * setVec4Uniform(const std::string &, const glm::vec4 &);
	ShaderProgram * setVec3Uniform(const std::string &, const glm::vec3 &);
	ShaderProgram * setMatrix4Uniform(const std::string &, const glm::mat4 &);

	static std::string getShaderTypeName(GLenum);
	static GLuint compileShader(GLenum, const std::string &);
	static GLuint compileShaderFromFile(GLenum, const std::string &);

	ShaderProgram * attach(GLuint);
	ShaderProgram * attachCode(GLenum, const std::string &);
	ShaderProgram * attachFile(GLenum, const std::string &);
	ShaderProgram * linkOrDelete();
};

