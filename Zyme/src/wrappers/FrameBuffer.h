#pragma once
#include <GL\glew.h>

class FrameBuffer {
private:
	GLuint value;

public:
	FrameBuffer();
	~FrameBuffer();

	FrameBuffer * bind();
	const FrameBuffer * bind() const;

	static void bindDefault();

	bool isComplete() const;

	FrameBuffer * drawBuffer(GLenum);
	FrameBuffer * readBuffer(GLenum);
};

