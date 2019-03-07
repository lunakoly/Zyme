#include "FrameBuffer.h"

FrameBuffer::FrameBuffer() {
	glGenFramebuffers(1, &value);
}

FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1, &value);
}

FrameBuffer * FrameBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, value);
	return this;
}

const FrameBuffer * FrameBuffer::bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, value);
	return this;
}

void FrameBuffer::bindDefault() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool FrameBuffer::isComplete() const {
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

FrameBuffer * FrameBuffer::drawBuffer(GLenum mode) {
	glDrawBuffer(mode);
	return this;
}

FrameBuffer * FrameBuffer::readBuffer(GLenum mode) {
	glReadBuffer(mode);
	return this;
}