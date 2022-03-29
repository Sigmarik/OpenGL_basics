#ifndef LABGAME_CAMERA_CLASS_H
#define LABGAME_CAMERA_CLASS_H

#define CAMERA_PERSPECTIVE 0
#define CAMERA_ORTHOGRAPHIC 1
#define CAMERA_EMPTY 2

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_projection.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_relational.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/vector_common.hpp>
#include <glm/vector_relational.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Shader.h"

class Camera {
public:
	glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 forward = glm::vec3(0.0, 0.0, 1.0);
	float fov = 3.1415926535 * (3.0 / 4.0);
	int displayWidth = 800;
	int displayHeight = 800;
	float orthoWidth = 10.0;
	float orthoHeight = 10.0;
	float nearPlane = 0.1;
	float farPlane = 100.0;
	float speed = 30;
	float sencitivity = 1.0;
	glm::vec2 lastCursorPos = glm::vec2(0.0);
	bool enabledRotation = false;
	int type = CAMERA_PERSPECTIVE;
	Camera(int cameraType = CAMERA_PERSPECTIVE);
	void pushMatrices(Shader& shader);
	void operateInputs(GLFWwindow* window, double deltaTime);
};

#endif