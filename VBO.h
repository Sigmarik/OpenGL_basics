#ifndef LABGAME_VBO_CLASS_H
#define LABGAME_VBO_CLASS_H

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

class Vertex {
public:
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec2 uv;
	glm::vec4 color;
	Vertex();
	Vertex(glm::vec3 pos, glm::vec3 _uv);
};

class VBO {
public:
	GLuint id;
	VBO();
	VBO(std::vector<Vertex>& vertices);
	void bind();
	void unbind();
	void destroy();
};

#endif