#include <string>
#include <fstream>
#include <GL\glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "..\auxiliary\Alias.h"
#include "ShaderProgram.h"

ShaderProgram::ShaderProgram() : value(glCreateProgram()) {

}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(value);
}

ShaderProgram * ShaderProgram::use() {
	glUseProgram(value);
	return this;
}

// SAME
const ShaderProgram * ShaderProgram::use() const {
	glUseProgram(value);
	return this;
}

ShaderProgram * ShaderProgram::setIntUniform(const std::string & name, GLint uniformValue) {
	const GLint target = glGetUniformLocation(value, name.c_str());
	glUniform1i(target, uniformValue);
	return this;
}

ShaderProgram * ShaderProgram::setFloatUniform(const std::string & name, GLfloat uniformValue) {
	const GLint target = glGetUniformLocation(value, name.c_str());
	glUniform1f(target, uniformValue);
	return this;
}

ShaderProgram * ShaderProgram::setVec4Uniform(const std::string & name, const glm::vec4 & vector) {
	const GLint target = glGetUniformLocation(value, name.c_str());
	glUniform4f(target, vector.x, vector.y, vector.z, vector.w);
	return this;
}

ShaderProgram * ShaderProgram::setVec3Uniform(const std::string & name, const glm::vec3 & vector) {
	const GLint target = glGetUniformLocation(value, name.c_str());
	glUniform3f(target, vector.x, vector.y, vector.z);
	return this;
}

ShaderProgram * ShaderProgram::setMatrix4Uniform(const std::string & name, const glm::mat4 & matrix) {
	const GLint target = glGetUniformLocation(value, name.c_str());
	glUniformMatrix4fv(target, 1, GL_FALSE, glm::value_ptr(matrix));
	return this;
}

std::string ShaderProgram::getShaderTypeName(GLenum type) {
	if (type == GL_VERTEX_SHADER)
		return "GL_VERTEX_SHADER";
	if (type == GL_FRAGMENT_SHADER)
		return "GL_FRAGMENT_SHADER";
	if (type == GL_GEOMETRY_SHADER)
		return "GL_GEOMETRY_SHADER";
	return "GL_COMPUTE_SHADER";
}

GLuint ShaderProgram::compileShader(GLenum type, const std::string & shaderCode) {
	const char * shaderCodeCStyle = shaderCode.c_str();
	const GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, &shaderCodeCStyle, nullptr);
	glCompileShader(shader);

	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	// error occured
	if (result == GL_FALSE) {
		// prepare free space for the message
		GLint length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char * message = new char[length];
		// get the message
		glGetShaderInfoLog(shader, length, &length, message);
		// remove trash
		glDeleteShader(shader);
		return ERROR << "Failed to compile " << getShaderTypeName(type) << ":\n" << message;
	}

	return shader;
}

GLuint ShaderProgram::compileShaderFromFile(GLenum type, const std::string & path) {
	std::ifstream file(path);

	if (!file) {
		return ERROR << "Could not open file " + path;
	}

	std::string code(
		(std::istreambuf_iterator<char>(file)),
		(std::istreambuf_iterator<char>())
	);

	return compileShader(type, code);
}

ShaderProgram * ShaderProgram::attach(GLuint shader) {
	glAttachShader(value, shader);
	return this;
}

ShaderProgram * ShaderProgram::attachCode(GLenum type, const std::string & code) {
	const GLuint shader = compileShader(type, code);
	glAttachShader(value, shader);
	glDeleteShader(shader);
	return this;
}

ShaderProgram * ShaderProgram::attachFile(GLenum type, const std::string & code) {
	const GLuint shader = compileShaderFromFile(type, code);
	glAttachShader(value, shader);
	glDeleteShader(shader);
	return this;
}

ShaderProgram * ShaderProgram::linkOrDelete() {
	glLinkProgram(value);
	//glValidateProgram(value);

	GLint result;
	glGetProgramiv(value, GL_LINK_STATUS, &result);

	// error occured
	if (result == GL_FALSE) {
		// prepare free space for the message
		GLint length;
		glGetProgramiv(value, GL_INFO_LOG_LENGTH, &length);
		char * message = new char[length];
		// get the message
		glGetProgramInfoLog(value, length, &length, message);
		LOG ERROR << "Failed to link program:\n" << message;
		// remove trash
		delete this;
		return nullptr;
	}

	return this;
}