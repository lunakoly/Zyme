#pragma once
#include <GL\glew.h>

class RenderBuffer {
private:
	GLuint value;

public:
	RenderBuffer();
	~RenderBuffer();

	RenderBuffer * bind();
	const RenderBuffer * bind() const;

	RenderBuffer * store(GLenum, GLsizei, GLsizei);

	RenderBuffer * attachToBoundFramebuffer(GLenum);
};