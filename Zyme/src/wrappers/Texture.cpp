#include <string>
#include <GL\glew.h>

#include "..\..\other\stb_image\stb_image.h"
#include "..\auxiliary\Alias.h"
#include "Texture.h"


Texture::Texture() {
	glGenTextures(1, &value);
}

Texture::~Texture() {
	glDeleteTextures(1, &value);
}

void Texture::targeting(GLenum slot) {
	glActiveTexture(slot);
}

GLuint Texture::getValue() {
	return value;
}

Texture * Texture::bindAs(GLenum target) {
	glBindTexture(target, value);
	this->target = target;
	return this;
}

Texture * Texture::unbind() {
	glBindTexture(target, 0);
	return this;
}

void Texture::unbindFrom(GLenum target) {
	glBindTexture(target, 0);
}

void Texture::tryOrDefault(GLenum target, Texture * texture, Texture * defaultTexture) {
	if (texture != nullptr)
		texture->bindAs(target);
	else
		defaultTexture->bindAs(target);
}

Texture * Texture::minify(GLint param) {
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, param);
	return this;
}

Texture * Texture::magnify(GLint param) {
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, param);
	return this;
}

Texture * Texture::wrapS(GLint param) {
	glTexParameteri(target, GL_TEXTURE_WRAP_S, param);
	return this;
}

Texture * Texture::wrapT(GLint param) {
	glTexParameteri(target, GL_TEXTURE_WRAP_T, param);
	return this;
}

Texture * Texture::allocate2D(
		GLint level, GLint internalFormat, 
		GLsizei width, GLsizei height, GLenum format, 
		GLenum type, const GLvoid * data) {
	glTexImage2D(
		target, level, internalFormat, 
		width, height, 0, format, 
		type, data);
	return this;
}

Texture * Texture::allocate2DFromFile(
		const std::string & path, 
		GLint level, GLint internalFromat,
		GLenum format, GLenum type) {
	int channels;
	int height;
	int width;

	stbi_uc * buffer = stbi_load(path.c_str(), &width, &height, &channels, 4);

	if (!buffer) {
		LOG ERROR << "Could not load texture from " << path;
		return this;
	}

	allocate2D(level, internalFromat, width, height, format, type, buffer);
	stbi_image_free(buffer);
	return this;
}

Texture * Texture::prepareSimpleTexture() {
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return this;
}

Texture * Texture::attachToBoundFramebuffer(GLenum attachment, GLenum target) {
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, target, value, 0);
	return this;
}