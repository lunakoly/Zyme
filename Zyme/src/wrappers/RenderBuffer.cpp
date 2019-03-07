#include <GL\glew.h>

#include "RenderBuffer.h"

RenderBuffer::RenderBuffer() {
	glGenRenderbuffers(1, &value);
}

RenderBuffer::~RenderBuffer() {
	glDeleteRenderbuffers(1, &value);
}

RenderBuffer * RenderBuffer::bind() {
	glBindRenderbuffer(GL_RENDERBUFFER, value);
	return this;
}

const RenderBuffer * RenderBuffer::bind() const {
	glBindRenderbuffer(GL_RENDERBUFFER, value);
	return this;
}

RenderBuffer * RenderBuffer::store(
		GLenum internalFormat,
		GLsizei width, GLsizei height) {
	glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
	return this;
}

RenderBuffer * RenderBuffer::attachToBoundFramebuffer(GLenum attachment) {
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, value);
	return this;
}