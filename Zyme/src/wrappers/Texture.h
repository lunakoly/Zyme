#pragma once
#include <string>
#include <GL\glew.h>

class Texture {
private:
	GLuint value = 0;
	GLenum target = 0;

public:
	Texture();
	~Texture();

	static void targeting(GLenum);

	GLuint getValue();

	Texture * bindAs(GLenum);
	Texture * unbind();

	static void unbindFrom(GLenum);
	static void tryOrDefault(GLenum, Texture *, Texture *);

	Texture * minify(GLint);
	Texture * magnify(GLint);
	Texture * wrapS(GLint);
	Texture * wrapT(GLint);

	Texture * allocate2D(
		GLint, GLint, GLsizei, GLsizei,
		GLenum, GLenum, const GLvoid *);

	Texture * allocate2DFromFile(
		const std::string & path,
		GLint, GLint, GLenum, GLenum);

	Texture * prepareSimpleTexture();

	Texture * attachToBoundFramebuffer(GLenum, GLenum);
};