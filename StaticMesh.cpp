#include "StaticMesh.h"

void StaticMesh::clear() {
	vertices.clear();
	faces.clear();
}

void StaticMesh::recalculateTangents() {
	// Do nothing for now
}

void StaticMesh::updateBuffers() {
	std::cout << "Pushing " << vertices.size() << " vertices and " << faces.size() << " faces" << std::endl;
	vao = VAO();
	vao.bind();
	vbo = VBO(vertices);
	ebo = EBO(faces);
	vao.assignData(vbo, 0, 3, sizeof(Vertex), (void*)0);
	vao.assignData(vbo, 1, 3, sizeof(Vertex), (void*)(3 * sizeof(float)));
	vao.assignData(vbo, 2, 3, sizeof(Vertex), (void*)(6 * sizeof(float)));
	vao.assignData(vbo, 3, 2, sizeof(Vertex), (void*)(9 * sizeof(float)));
	vao.assignData(vbo, 4, 4, sizeof(Vertex), (void*)(11 * sizeof(float)));
	vao.unbind();
	vbo.unbind();
	ebo.unbind();
}

void StaticMesh::recalculateNormals() {
	std::vector<glm::vec3> meaned(vertices.size(), glm::vec3(0.0));
	for (int i = 0; i < faces.size(); i += 3) {
		int v_a = faces[i], v_b = faces[i + 1], v_c = faces[i + 2];
		glm::vec3 normal = -glm::normalize(glm::cross(vertices[v_b].position - vertices[v_a].position,
			vertices[v_c].position - vertices[v_a].position));
		float cosA = glm::dot(glm::normalize(vertices[v_b].position - vertices[v_a].position),
			glm::normalize(vertices[v_c].position - vertices[v_a].position));
		float cosB = glm::dot(glm::normalize(vertices[v_a].position - vertices[v_b].position),
			glm::normalize(vertices[v_c].position - vertices[v_b].position));
		float cosC = glm::dot(glm::normalize(vertices[v_b].position - vertices[v_c].position),
			glm::normalize(vertices[v_a].position - vertices[v_c].position));
		meaned[v_a] += normal * std::acos(cosA);
		meaned[v_b] += normal * std::acos(cosB);
		meaned[v_c] += normal * std::acos(cosC);
	}
	for (int index = 0; index < vertices.size(); index++) {
		vertices[index].normal = glm::normalize(meaned[index]);
	}
}

void StaticMesh::toSharp() {
	std::vector<bool> needsDupe(vertices.size(), false);
	for (int face_i = 0; face_i < faces.size(); face_i++) {
		GLuint v_i = faces[face_i];
		if (!needsDupe[v_i]) {
			needsDupe[v_i] = true;
			continue;
		}
		vertices.push_back(vertices[v_i]);
		needsDupe.push_back(true);
		faces[face_i] = vertices.size() - 1;
	}
	recalculateNormals();
}

void StaticMesh::optimize() {
	std::vector<bool> used(vertices.size(), false);
	std::vector<Vertex> oldVertices = vertices;
	vertices.clear();
	for (int i = 0; i < faces.size(); i++) {
		used[faces[i]] = true;
	}
	for (int i = 0; i < oldVertices.size(); i++) {
		if (used[i]) vertices.push_back(oldVertices[i]);
	}
}

void StaticMesh::toSmooth(float eps) {
	std::vector<int> zipmap(vertices.size(), -1);
	for (int v_i = 0; v_i < faces.size(); v_i++) {
		for (int i = 0; i < faces.size(); i++) {
			if (glm::distance(vertices[faces[v_i]].position, vertices[faces[i]].position) > eps ||
				glm::distance(vertices[faces[v_i]].uv, vertices[faces[i]].uv) > eps || faces[i] == faces[v_i]) continue;
			if (zipmap[faces[i]] == -1) {
				zipmap[faces[v_i]] = i;
			}
			else {
				zipmap[faces[v_i]] = zipmap[faces[i]];
			}
			faces[v_i] = zipmap[faces[v_i]];
		}
	}
	optimize();
	recalculateNormals();
}

void StaticMesh::loadFile(const char* fname) {
	std::fstream stream;
	stream.open(fname, std::ios::in);
	int vertsCount, faceCount;
	stream >> vertsCount >> faceCount;
	vertices = std::vector<Vertex>(vertsCount, Vertex());
	faces = std::vector<GLuint>(faceCount * 3, 0);
	for (int i = 0; i < vertsCount; i++) {
		float x, y, z;
		stream >> x >> y >> z;
		vertices[i].position = glm::vec3(x, y, z);
		//std::cout << x << ' ' << y << ' ' << z << std::endl;
	}
	std::string normalKey;
	stream >> normalKey;
	if (normalKey == "do_normals") {
		for (int i = 0; i < vertsCount; i++) {
			float x, y, z;
			stream >> x >> y >> z;
			vertices[i].normal = glm::vec3(x, y, z);
		}
	}
	for (int i = 0; i < vertsCount; i++) {
		float x, y;
		stream >> x >> y;
		vertices[i].uv = glm::vec2(x, y);
	}
	for (int i = 0; i < vertsCount; i++) {
		float r, g, b, a;
		stream >> r >> g >> b >> a;
		vertices[i].color.r = r;
		vertices[i].color.g = g;
		vertices[i].color.b = b;
		vertices[i].color.a = a;
	}
	for (int i = 0; i < faceCount * 3; i++) {
		int index;
		stream >> index;
		faces[i] = index;
	}
	if (normalKey == "no_normals") {
		recalculateNormals();
	}
	//std::cout << vertices.size() << ' ' << faces.size() << std::endl;
}

StaticMesh::StaticMesh() {
	location = glm::vec3(0.0, 0.0, 0.0);
	rotation = glm::vec3(0.0, 0.0, 0.0);
	scale = glm::vec3(1.0, 1.0, 1.0);
	updateBuffers();
}

void StaticMesh::draw(Camera& camera, Shader& shader) {
	shader.activate();
	vao.bind();
	GLuint tformMatrixUniID = glGetUniformLocation(shader.id, "objectTranslateMatrix");
	camera.pushMatrices(shader);
	glUniformMatrix4fv(tformMatrixUniID, 1, GL_FALSE, glm::value_ptr(
		glm::translate(location) * glm::scale(scale)));
	glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0);
	vao.unbind();
}

void StaticMesh::destroy() {
	vao.destroy();
	vbo.destroy();
	ebo.destroy();
}