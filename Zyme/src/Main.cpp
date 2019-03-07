#include <iostream>

#include "RenderingContext.h"

#include "util\Time.h"

#include "entities\Entity.h"

#include "services\GameService.h"


static Buffer<GLfloat> * cubeVBO;
static Buffer<GLuint> * cubeIBO;
static VertexArray * cubeVAO;

static Texture * mvTexture;
static Texture * physicsTexture;
static Texture * physicsNormal;
static Texture * physicsSpecular;
static Texture * greenWoolTexture;
static Texture * greenWoolNormal;

static Entity * scene;
static Mesh * commonMesh;

static Entity * ground;
static Transform * groundTransform;
static Material * groundMaterial;

static Entity * cube;
static Transform * cubeTransform;
static Material * cubeMaterial;

static Entity * cube2;
static Transform * cube2Transform;
static Material * cube2Material;

static Entity * grave;
static Transform * graveTransform;
static Material * graveMaterial;

static Entity * aSun;
static Transform * aSunTransform;
static DirectionalLight * aSunLight;

static Entity * camera;
static Camera * cameraCamera;
static Transform * cameraTransform;


void prepare() {
	std::cout << "Window size: (" << RenderingContext::windowWidth << ", " << RenderingContext::windowHeight << ')' << std::endl;
	std::cout << "OpenGL version: " << RenderingContext::getOpenGLVersion() << std::endl;

	const GLfloat cubeVertices[] = {
		/*   POSITION      |     UV      |       NORMAL       |      TANGENT  */

		// front
		-1.0,  1.0, -1.0,     0.0, 1.0,     0.0,  0.0, -1.0,     1.0,  0.0,  0.0,
		 1.0,  1.0, -1.0,     1.0, 1.0,     0.0,  0.0, -1.0,     1.0,  0.0,  0.0,
		-1.0, -1.0, -1.0,     0.0, 0.0,     0.0,  0.0, -1.0,     1.0,  0.0,  0.0,
		 1.0, -1.0, -1.0,     1.0, 0.0,     0.0,  0.0, -1.0,     1.0,  0.0,  0.0,

		// back
		 1.0,  1.0,  1.0,     0.0, 1.0,     0.0,  0.0,  1.0,    -1.0,  0.0,  0.0,
		-1.0,  1.0,  1.0,     1.0, 1.0,     0.0,  0.0,  1.0,    -1.0,  0.0,  0.0,
		 1.0, -1.0,  1.0,     0.0, 0.0,     0.0,  0.0,  1.0,    -1.0,  0.0,  0.0,
		-1.0, -1.0,  1.0,     1.0, 0.0,     0.0,  0.0,  1.0,    -1.0,  0.0,  0.0,

		// right
		 1.0,  1.0, -1.0,     0.0, 1.0,     1.0,  0.0,  0.0,     0.0,  0.0,  1.0,
		 1.0,  1.0,	 1.0,     1.0, 1.0,     1.0,  0.0,  0.0,     0.0,  0.0,  1.0,
		 1.0, -1.0, -1.0,     0.0, 0.0,     1.0,  0.0,  0.0,     0.0,  0.0,  1.0,
		 1.0, -1.0,  1.0,     1.0, 0.0,     1.0,  0.0,  0.0,     0.0,  0.0,  1.0,
	
		// left
		-1.0,  1.0,  1.0,     0.0, 1.0,    -1.0,  0.0,  0.0,     0.0,  0.0, -1.0,
		-1.0,  1.0,	-1.0,     1.0, 1.0,    -1.0,  0.0,  0.0,     0.0,  0.0, -1.0,
		-1.0, -1.0,  1.0,     0.0, 0.0,    -1.0,  0.0,  0.0,     0.0,  0.0, -1.0,
		-1.0, -1.0, -1.0,     1.0, 0.0,    -1.0,  0.0,  0.0,     0.0,  0.0, -1.0,

		// top
		-1.0,  1.0,  1.0,     0.0, 1.0,     0.0,  1.0,  0.0,     1.0,  0.0,  0.0,
		 1.0,  1.0,	 1.0,     1.0, 1.0,     0.0,  1.0,  0.0,     1.0,  0.0,  0.0,
		-1.0,  1.0, -1.0,     0.0, 0.0,     0.0,  1.0,  0.0,     1.0,  0.0,  0.0,
		 1.0,  1.0, -1.0,     1.0, 0.0,     0.0,  1.0,  0.0,     1.0,  0.0,  0.0,

		// bottom
		-1.0, -1.0, -1.0,     0.0, 1.0,     0.0, -1.0,  0.0,     1.0,  0.0,  0.0,
		 1.0, -1.0,	-1.0,     1.0, 1.0,     0.0, -1.0,  0.0,     1.0,  0.0,  0.0,
		-1.0, -1.0,  1.0,     0.0, 0.0,     0.0, -1.0,  0.0,     1.0,  0.0,  0.0,
		 1.0, -1.0,  1.0,     1.0, 0.0,     0.0, -1.0,  0.0,     1.0,  0.0,  0.0,
	};

	cubeVBO = (new Buffer<GLfloat>(GL_FLOAT))
		->bindAs(GL_ARRAY_BUFFER)
		->fillWith(264, cubeVertices);

	const GLuint cubeIndices[] = {
		// front
		1, 0, 2,
		1, 2, 3,

		// back
		5, 4, 6,
		5, 6, 7,

		// right
		9, 8, 10,
		9, 10, 11,

		// left
		13, 12, 14,
		13, 14, 15,

		// top
		17, 16, 18,
		17, 18, 19,

		// bottom
		21, 20, 22,
		21, 22, 23
	};

	cubeIBO = (new Buffer<GLuint>(GL_UNSIGNED_INT))
		->bindAs(GL_ELEMENT_ARRAY_BUFFER)
		->fillWith(36, cubeIndices);

	cubeVAO = (new VertexArray())
		->bind()
		->layout(cubeVBO, 3, GL_FALSE, 11, 0)
		->layout(cubeVBO, 2, GL_FALSE, 11, 3)
		->layout(cubeVBO, 3, GL_FALSE, 11, 5)
		->layout(cubeVBO, 3, GL_FALSE, 11, 8);

	mvTexture = (new Texture())
		->bindAs(GL_TEXTURE_2D)
		->prepareSimpleTexture()
		//->allocate2DFromFile("res/textures/mv.png", 0, GL_SRGB_ALPHA, GL_RGBA, GL_UNSIGNED_BYTE)
		->allocate2DFromFile("res/textures/mv.png", 0, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE)
		->unbind();

	physicsTexture = (new Texture())
		->bindAs(GL_TEXTURE_2D)
		->prepareSimpleTexture()
		->minify(GL_LINEAR)
		->magnify(GL_LINEAR)
		//->allocate2DFromFile("res/textures/physics.png", 0, GL_SRGB_ALPHA, GL_RGBA, GL_UNSIGNED_BYTE)
		->allocate2DFromFile("res/textures/physics.png", 0, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE)
		->unbind();

	physicsNormal = (new Texture())
		->bindAs(GL_TEXTURE_2D)
		->prepareSimpleTexture()
		->minify(GL_LINEAR)
		->magnify(GL_LINEAR)
		->allocate2DFromFile("res/textures/physics_normal.png", 0, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE)
		->unbind();

	physicsSpecular = (new Texture())
		->bindAs(GL_TEXTURE_2D)
		->prepareSimpleTexture()
		->minify(GL_LINEAR)
		->magnify(GL_LINEAR)
		->allocate2DFromFile("res/textures/physics_specular.png", 0, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE)
		->unbind();

	greenWoolTexture = (new Texture())
		->bindAs(GL_TEXTURE_2D)
		->prepareSimpleTexture()
		//->allocate2DFromFile("res/textures/green_wool.png", 0, GL_SRGB_ALPHA, GL_RGBA, GL_UNSIGNED_BYTE)
		->allocate2DFromFile("res/textures/green_wool.png", 0, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE)
		->unbind();

	greenWoolNormal = (new Texture())
		->bindAs(GL_TEXTURE_2D)
		->prepareSimpleTexture()
		->allocate2DFromFile("res/textures/green_wool_normal.png", 0, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE)
		->unbind();


	scene = new Entity();
	commonMesh = new Mesh(cubeVAO, cubeIBO);


	ground = new Entity();
	scene->addChild(ground);

	groundTransform = new Transform();
	groundTransform->size.z = 20;
	groundTransform->size.x = 20;
	groundTransform->size.y = 0.1;
	groundTransform->position.y = -3;

	groundMaterial = new Material();
	groundMaterial->color = glm::vec4(0.7, 0.7, 0.7, 1.0);

	ground->addComponent(commonMesh);
	ground->addComponent(groundMaterial);
	ground->addComponent(groundTransform);


	cube = new Entity();
	scene->addChild(cube);

	cubeTransform = new Transform();
	cubeTransform->position.z = 7;
	cubeTransform->position.y = -1;
	cubeTransform->view.set(glm::radians(45.0), glm::radians(-10.0), 0.0);
	
	cubeMaterial = new Material();
	cubeMaterial->color = glm::vec4(0.5, 0.7, 1.0, 1.0);
	cubeMaterial->diffuse = mvTexture;

	cube->addComponent(commonMesh);
	cube->addComponent(cubeMaterial);
	cube->addComponent(cubeTransform);
	

	cube2 = new Entity();
	scene->addChild(cube2);

	cube2Transform = new Transform();
	cube2Transform->position.z = 10;
	cube2Transform->view.set(glm::radians(-45.0), glm::radians(-45.0), 0.0);

	cube2Material = new Material();
	cube2Material->color = glm::vec4(0.7, 1.0, 0.7, 1.0);
	cube2Material->diffuse = greenWoolTexture;
	cube2Material->normal = greenWoolNormal;

	cube2->addComponent(commonMesh);
	cube2->addComponent(cube2Material);
	cube2->addComponent(cube2Transform);
	
	
	grave = new Entity();
	scene->addChild(grave);

	graveTransform = new Transform();
	graveTransform->position.y = -2.0;
	graveTransform->position.z = 11;
	graveTransform->position.x = 4;
	graveTransform->size.z = 0.2;
	graveTransform->view.set(glm::radians(-30.0), 0.0, 0.0);

	graveMaterial = new Material();
	graveMaterial->color = glm::vec4(1.0, 0.8, 0.6, 1.0);
	graveMaterial->diffuse = physicsTexture;
	graveMaterial->normal = physicsNormal;
	graveMaterial->specular = physicsSpecular;

	grave->addComponent(commonMesh);
	grave->addComponent(graveMaterial);
	grave->addComponent(graveTransform);


	aSun = new Entity();
	scene->addChild(aSun);

	aSunTransform = new Transform();
	aSunTransform->position.y = 2;
	aSunTransform->position.x = 4;
	aSunTransform->position.z = 6;
	aSunTransform->view.set(glm::radians(-45.0), glm::radians(45.0), 0.0);

	aSunLight = new DirectionalLight();

	aSun->addComponent(aSunLight);
	aSun->addComponent(aSunTransform);


	camera = new Entity();
	scene->addChild(camera);

	cameraTransform = new Transform();

	glm::mat4 zFlipper = glm::scale(glm::mat4(1.0), glm::vec3(1.0, 1.0, -1.0));
	float aspect = RenderingContext::windowWidth / (float) RenderingContext::windowHeight;

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
	cameraCamera = new Camera(projection * zFlipper, true);

	camera->addComponent(cameraCamera);
	camera->addComponent(cameraTransform);
}

void update() {
	cubeTransform ->view.add(glm::radians(45.0 * Time::deltaTime), glm::radians(45.0 * Time::deltaTime), 0.0);
	cube2Transform->view.add(glm::radians(45.0 * Time::deltaTime), 0.0, 0.0);
}

int main() {
	if (
		!RenderingContext::initialize() ||
		!GameService::initialize()
	) {
		std::cout << "Terminate > ";
		std::cin.get();
		return -1;
	}

	prepare();

	while (RenderingContext::proceed()) {
		update();
		GameService::update(scene);
	}

	RenderingContext::terminate();
}