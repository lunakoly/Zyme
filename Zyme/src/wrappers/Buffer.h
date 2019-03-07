#pragma once
#include <GL\glew.h>

template <typename T>
class Buffer {
private:
	GLuint value = 0;
	GLenum target = 0;
	GLenum usage = GL_STATIC_DRAW;
	GLsizei count = 0;

public:
	const GLenum type;
	
	Buffer(GLenum type) : type(type) {
		glGenBuffers(1, &value);
	}

	~Buffer() {
		glDeleteBuffers(1, &value);
	}

	Buffer<T> * bindAs(GLenum target) {
		glBindBuffer(target, value);
		this->target = target;
		return this;
	}

	Buffer<T> * assuming(GLenum usage) {
		this->usage = usage;
		return this;
	}

	Buffer<T> * fillWith(GLsizei count, const T * data) {
		glBufferData(target, count * sizeof(T), data, usage);
		this->count = count;
		return this;
	}

	Buffer<T> * drawElements(GLenum mode) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, value);
		glDrawElements(mode, count, type, nullptr);
		return this;
	}

	const Buffer<T> * drawElements(GLenum mode) const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, value);
		glDrawElements(mode, count, type, nullptr);
		return this;
	}
};
