#ifndef LABGAME_STATICMESH_CLASS_H
#define LABGAME_STATICMESH_CLASS_H

#include <glad/glad.h>

#include "EBO.h"
#include "VBO.h"
#include "VAO.h"

#include "Camera.h"
#include "Shader.h"

class StaticMesh {
public:
	VBO vbo;
	VAO vao;
	EBO ebo;
	std::vector<GLuint> faces;
	std::vector<Vertex> vertices;
	glm::vec3 location = glm::vec3(0.0);
	glm::mat4 rotation = glm::mat4(1.0);
	glm::vec3 scale = glm::vec3(1.0);
	StaticMesh();
	void loadFile(const char* fname);
	void updateBuffers();
	void clear();
	void recalculateNormals();
	void recalculateTangents();
	void toSharp();
	void toSmooth(float eps = 0.0001);
	void optimize();
	void draw(Camera& camera, Shader& shader);
	void destroy();
};

#endif