#include "VBO.h"

Vertex::Vertex() {}

Vertex::Vertex(glm::vec3 pos, glm::vec3 _uv) {
	position = pos;
	uv = _uv;
}

VBO::VBO() {
	glGenBuffers(1, &id);
}

VBO::VBO(std::vector<Vertex>& vertices) {
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VBO::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::destroy() {
	glDeleteBuffers(1, &id);
}