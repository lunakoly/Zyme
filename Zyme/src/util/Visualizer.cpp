#include "Visualizer.h"
#include "RenderingContext.h"

#include "auxiliary\Alias.h"

#include "wrappers\ShaderProgram.h"
#include "wrappers\FrameBuffer.h"
#include "wrappers\VertexArray.h"
#include "wrappers\Texture.h"
#include "wrappers\Buffer.h"


static Buffer<GLfloat> * visualizationVerticesBuffer;
static Buffer<GLubyte> * visualizationIndicesBuffer;
static VertexArray * visualizationVAO;

static ShaderProgram * colorTextureShaderProgram;
static ShaderProgram * depthTextureShaderProgram;


bool Visualizer::initialize() {
	const GLfloat vertices[] = {
		 1.0,  1.0,
		-1.0,  1.0,
		-1.0, -1.0,
		 1.0, -1.0
	};

	visualizationVerticesBuffer = (new Buffer<GLfloat>(GL_FLOAT))
		->bindAs(GL_ARRAY_BUFFER)
		->fillWith(8, vertices);

	const GLubyte indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	visualizationIndicesBuffer = (new Buffer<GLubyte>(GL_UNSIGNED_BYTE))
		->bindAs(GL_ELEMENT_ARRAY_BUFFER)
		->fillWith(6, indices);

	colorTextureShaderProgram = (new ShaderProgram())
		->attachFile(GL_VERTEX_SHADER, "src/shaders/ColorTexture.vert")
		->attachFile(GL_FRAGMENT_SHADER, "src/shaders/ColorTexture.frag")
		->linkOrDelete();

	depthTextureShaderProgram = (new ShaderProgram())
		->attachFile(GL_VERTEX_SHADER, "src/shaders/DepthTexture.vert")
		->attachFile(GL_FRAGMENT_SHADER, "src/shaders/DepthTexture.frag")
		->linkOrDelete();

	if (
		colorTextureShaderProgram == nullptr ||
		depthTextureShaderProgram == nullptr
	) return ERROR << "Could not initialize Visualizer";

	// set up the vao
	visualizationVAO = (new VertexArray())
		->bind()
		->layout(visualizationVerticesBuffer, 2, GL_FALSE, 2, 0);

	return true;
}

void Visualizer::terminate() {
	delete visualizationIndicesBuffer;
	delete visualizationVerticesBuffer;
	delete visualizationVAO;

	delete colorTextureShaderProgram;
	delete depthTextureShaderProgram;
}

void Visualizer::displayTexture(Texture * texture) {
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glCullFace(GL_BACK);

	FrameBuffer::bindDefault();
	visualizationVAO->bind();

	Texture::targeting(GL_TEXTURE0);
	texture->bindAs(GL_TEXTURE_2D);
	
	colorTextureShaderProgram
		->use()
		->setIntUniform("uTexture", 0);
	
	glViewport(0, 0, RenderingContext::windowWidth, RenderingContext::windowHeight);

	visualizationIndicesBuffer
		->drawElements(GL_TRIANGLES);
}

void Visualizer::displayDepth(Texture * texture) {
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glCullFace(GL_BACK);

	FrameBuffer::bindDefault();
	visualizationVAO->bind();

	Texture::targeting(GL_TEXTURE0);
	texture->bindAs(GL_TEXTURE_2D);
	
	depthTextureShaderProgram
		->use()
		->setIntUniform("uTexture", 0);

	glViewport(0, 0, RenderingContext::windowWidth, RenderingContext::windowHeight);

	visualizationIndicesBuffer
		->drawElements(GL_TRIANGLES);
}