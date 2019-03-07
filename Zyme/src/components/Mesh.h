#pragma once

#include "wrappers\Buffer.h"
#include "wrappers\VertexArray.h"

#include "Component.h"


/**
	Holds shape information required by
	MeshRendererSystem
*/
struct Mesh : public Component {
	VertexArray    * vertexArray;
	Buffer<GLuint> * indices;

	Mesh(VertexArray * vertexArray, Buffer<GLuint> * indices)
		: vertexArray(vertexArray)
		, indices(indices) {}
};
