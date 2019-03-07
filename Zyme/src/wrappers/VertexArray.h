#pragma once
#include <GL\glew.h>
#include "Buffer.h"

class VertexArray {
private:
	GLuint value;
	GLuint index = 0;

public:
	VertexArray();
	~VertexArray();

	VertexArray * bind();

	template <typename T>
	VertexArray * layout(
			Buffer<T> * buffer, GLint count,
			GLboolean normalized,
			GLsizei stride, GLsizei offset) {
		buffer->bindAs(GL_ARRAY_BUFFER);
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(
			index++, count, buffer->type, normalized,
			sizeof(T) * stride,
			(const GLvoid *) (sizeof(T) * offset));
		return this;
	}
};