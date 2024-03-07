#pragma once

#include "geometry_factory.hpp"

#include "ogl_resource.hpp"

struct IndexedBuffer {
	OpenGLResource vbo;
	OpenGLResource ebo;
	OpenGLResource vao;
};

inline IndexedBuffer
generateCubeBuffers() {
	IndexedBuffer buffers {
		createBuffer(),
		createBuffer(),
		createVertexArray()
	};

	// Cube vertices: 3 for vertex position.
	// Each face of the cube is made of 2 triangles, so 6 vertices per face
	float vertices[] = {
		// Positions
		-0.5f, -0.5f, -0.5f,  // 0. Back face
		0.5f, -0.5f, -0.5f,  // 1.
		-0.5f,  0.5f, -0.5f,  // 2.
		0.5f,  0.5f, -0.5f,  // 3.
		-0.5f, -0.5f,  0.5f,  // 4. Front face
		0.5f, -0.5f,  0.5f,  // 5.
		-0.5f,  0.5f,  0.5f,  // 6.
		0.5f,  0.5f,  0.5f   // 7.
	};

	unsigned int indices[] = {
		// Back face
		0, 1, 3, 0, 3, 2,
		// Front face
		4, 5, 7, 4, 7, 6,
		// Left face
		0, 4, 6, 0, 6, 2,
		// Right face
		1, 5, 7, 1, 7, 3,
		// Bottom face
		0, 4, 5, 0, 5, 1,
		// Top face
		2, 6, 7, 2, 7, 3
	};


	GL_CHECK(glBindVertexArray(buffers.vao.get()));

	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, buffers.vbo.get()));
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.ebo.get()));
	GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	// Position attribute
	GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
	GL_CHECK(glEnableVertexAttribArray(0));

	// Unbind VAO
	GL_CHECK(glBindVertexArray(0));

	return buffers;
}


class OGLGeometry: public AGeometry {
public:
	OGLGeometry(IndexedBuffer buff) :
		buffer(std::move(buff))
	{}
	IndexedBuffer buffer;
};

class OGLGeometryFactory: public GeometryFactory {
public:
	std::shared_ptr<AGeometry> getCube() {
		auto it = mObjects.find("cube");
		if (it != mObjects.end()) {
			return it->second;
		}

		auto geometry = std::make_shared<OGLGeometry>(generateCubeBuffers());
		mObjects["cube"] = geometry;
		return geometry;
	}

protected:
	std::map<std::string, std::shared_ptr<OGLGeometry>> mObjects;
};