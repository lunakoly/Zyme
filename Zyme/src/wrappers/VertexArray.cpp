#include "VertexArray.h"

VertexArray::VertexArray() {
	glGenVertexArrays(1, &value);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &value);
}

VertexArray * VertexArray::bind() {
	glBindVertexArray(value);
	return this;
}