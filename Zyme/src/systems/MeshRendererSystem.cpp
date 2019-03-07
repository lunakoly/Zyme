#include "RenderingContext.h"

#include "util\Time.h"

#include "MeshRendererSystem.h"


static ShaderProgram * depthShaderProgram;
static ShaderProgram * ambientShaderProgram;
static ShaderProgram * diffuseShaderProgram;

static Texture * defaultDiffuse;
static Texture * defaultNormal;
static Texture * testTexture;

static FrameBuffer * shadowsFramebuffer;

// a way to capture pointer to the scene
// and pass it to callbacks
static Entity * scene;

static glm::mat4 vpMatrix;
static glm::mat4 vpLightMatrix;
static glm::vec4 cameraPosition;


bool MeshRendererSystem::initialize() {
	// initialize shaders
	depthShaderProgram = (new ShaderProgram())
		->attachFile(GL_VERTEX_SHADER, "src/shaders/Depth.vert")
		->linkOrDelete();

	ambientShaderProgram = (new ShaderProgram())
		->attachFile(GL_VERTEX_SHADER, "src/shaders/Ambient.vert")
		->attachFile(GL_FRAGMENT_SHADER, "src/shaders/Ambient.frag")
		->linkOrDelete();

	diffuseShaderProgram = (new ShaderProgram())
		->attachFile(GL_VERTEX_SHADER, "src/shaders/Diffuse.vert")
		->attachFile(GL_FRAGMENT_SHADER, "src/shaders/Diffuse.frag")
		->linkOrDelete();

	if (
		depthShaderProgram   == nullptr ||
		ambientShaderProgram == nullptr || 
		diffuseShaderProgram == nullptr
	) return ERROR << "Could not initialize MeshRendererSystem";

	// initialize default transparent texture
	const GLubyte defaultDiffuseData[1][1][4] = {{{0, 0, 0, 0}}};

	defaultDiffuse = (new Texture())
		->bindAs(GL_TEXTURE_2D)
		->prepareSimpleTexture()
		->allocate2D(0, GL_RGBA8, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &defaultDiffuseData[0][0][0])
		->unbind();

	// initialize default normal texture
	const GLubyte defaultNormalData[1][1][4] = { {{128, 128, 255, 0}} };

	defaultNormal = (new Texture())
		->bindAs(GL_TEXTURE_2D)
		->prepareSimpleTexture()
		->allocate2D(0, GL_RGBA8, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &defaultNormalData[0][0][0])
		->unbind();

	// initialize shadows FBO
	shadowsFramebuffer = (new FrameBuffer())
		->bind()
		->drawBuffer(GL_NONE);

	// initialize test texture
	testTexture = (new Texture())
		->bindAs(GL_TEXTURE_2D)
		->prepareSimpleTexture()
		->allocate2DFromFile("res/textures/mv.png", 0, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE)
		->unbind();

	return true;
}

void MeshRendererSystem::terminate() {
	delete depthShaderProgram;
	delete diffuseShaderProgram;
	delete ambientShaderProgram;
	
	delete defaultDiffuse;
	delete defaultNormal;
	delete testTexture;

	delete shadowsFramebuffer;
}

static bool findCamera(
	Entity * it,
	Transform * transform,
	Camera * camera
) {
	if (camera->isActive) {
		cameraPosition = transform->model * glm::vec4(0.0, 0.0, 0.0, 1.0);
		vpMatrix = camera->projection * transform->inverse;
		return true;
	}

	return false;
}

static void renderAmbientForEntity(
	Entity * it,
	Transform * transform,
	Material * material,
	Mesh * mesh
) {
	// attach non-material data
	ambientShaderProgram
		->setMatrix4Uniform("uMVPMatrix", vpMatrix * transform->model);

	// attach required material data
	Texture::targeting(GL_TEXTURE0);
	Texture::tryOrDefault(GL_TEXTURE_2D, material->diffuse, defaultDiffuse);

	ambientShaderProgram
		->setIntUniform("uMaterial.diffuse", 0)
		->setVec4Uniform("uMaterial.color", material->color);
		
	// draw
	mesh->vertexArray->bind();
	mesh->indices->drawElements(GL_TRIANGLES);
}

static void renderAmbient() {
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ZERO);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	FrameBuffer::bindDefault();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, RenderingContext::windowWidth, RenderingContext::windowHeight);

	ambientShaderProgram
		->use()
		->setFloatUniform("uTime", Time::get());

	scene->traverseWith(&renderAmbientForEntity);
}

static void renderShadowImpact(
	Entity * it,
	Transform * transform,
	Mesh * mesh
) {
	// attach non-material data
	depthShaderProgram
		->setMatrix4Uniform("uMVPMatrix", vpLightMatrix * transform->model);
			
	// draw
	mesh->vertexArray->bind();
	mesh->indices->drawElements(GL_TRIANGLES);
}

static void renderDirectionalShadowMapForLight(
	Entity * it,
	Transform * transform,
	DirectionalLight * light
) {
	// light view-projection matrix
	vpLightMatrix = glm::scale(glm::mat4(1.0), glm::vec3(0.1, 0.1, 0.01)) * transform->inverse;

	// attach light's depth map
	light->depthTexture.attachToBoundFramebuffer(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D);
	glClear(GL_DEPTH_BUFFER_BIT);

	// hope, that framebuffer is complete now

	scene->traverseWith(&renderShadowImpact);
}

static void renderDirectionalShadowMaps() {
	shadowsFramebuffer->bind();
	glViewport(0, 0, 512, 512);

	// lil trick:
	// instead of adding bias to the value from
	// the depth texture in the fragment shader
	// we create depths for back faces.
	// that works as a much bigger automatic
	// bias that does not result in peter panning!
	glCullFace(GL_FRONT);

	depthShaderProgram
		->use()
		->setFloatUniform("uTime", Time::get());

	scene->traverseWith(&renderDirectionalShadowMapForLight);
}

static void renderDiffuseForEntity(
	Entity * it,
	Transform * transform,
	Material * material,
	Mesh * mesh
) {
	// attach non-material data
	diffuseShaderProgram
		->setMatrix4Uniform("uMLPMatrix", vpLightMatrix * transform->model)
		->setMatrix4Uniform("uMVPMatrix", vpMatrix * transform->model)
		->setMatrix4Uniform("uModelMatrix", transform->model);

	// attach required material data
	Texture::targeting(GL_TEXTURE0);
	Texture::tryOrDefault(GL_TEXTURE_2D, material->diffuse, defaultDiffuse);

	// attach required material data
	Texture::targeting(GL_TEXTURE1);
	Texture::tryOrDefault(GL_TEXTURE_2D, material->specular, defaultDiffuse);

	Texture::targeting(GL_TEXTURE2);
	Texture::tryOrDefault(GL_TEXTURE_2D, material->normal, defaultNormal);

	diffuseShaderProgram
		->setIntUniform("uMaterial.diffuse", 0)
		->setIntUniform("uMaterial.specular", 1)
		->setIntUniform("uMaterial.normal", 2)
		->setVec4Uniform("uMaterial.color", material->color)
		->setFloatUniform("uMaterial.shininess", material->shininess);

	// draw
	mesh->vertexArray->bind();
	mesh->indices->drawElements(GL_TRIANGLES);
}

static void renderDiffuseForLight(
	Entity * it,
	Transform * transform,
	DirectionalLight * light
) {
	// light view-projection matrix
	vpLightMatrix = glm::scale(glm::mat4(1.0), glm::vec3(0.1, 0.1, 0.01)) * transform->inverse;

	// attach light's depth map
	Texture::targeting(GL_TEXTURE3);
	light->depthTexture.bindAs(GL_TEXTURE_2D);

	// also precalculate light direction
	// here for optimization
	diffuseShaderProgram
		->setIntUniform("uLightDepthTexture", 3)
		->setVec4Uniform("uLightDirection", transform->model * glm::vec4(0.0, 0.0, 1.0, 0.0));

	scene->traverseWith(&renderDiffuseForEntity);
}

static void renderDiffuse() {
	// prepare to render diffuse
	FrameBuffer::bindDefault();
	glViewport(0, 0, RenderingContext::windowWidth, RenderingContext::windowHeight);
	glBlendFunc(GL_ONE, GL_ONE);
	glCullFace(GL_BACK);

	diffuseShaderProgram
		->use()
		->setFloatUniform("uTime", Time::get())
		->setVec4Uniform("uCameraPosition", cameraPosition);

	scene->traverseWith(&renderDiffuseForLight);
}

void MeshRendererSystem::update(Entity * scene) {
	::scene = scene;
	
	scene->find(&findCamera);

	renderAmbient();
	renderDirectionalShadowMaps();
	renderDiffuse();
}