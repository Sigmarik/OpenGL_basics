#include "Camera.h"


Camera::Camera(int cameraType) {
	type = cameraType;
}

void Camera::pushMatrices(Shader& shader) {
	glm::mat4 toView(1.0);
	glm::mat4 toSSpace(1.0);
	toView = glm::lookAt(position, position + forward, up);
	if (type == CAMERA_PERSPECTIVE) {
		toSSpace = glm::perspective(fov, (float)displayWidth / (float)displayHeight, nearPlane, farPlane);
	}
	else if (type == CAMERA_ORTHOGRAPHIC) {
		toSSpace = glm::ortho(-orthoWidth / 2.0, orthoWidth / 2.0, -orthoHeight / 2.0, orthoHeight / 2.0);
	}
	glUniformMatrix4fv(shader.uniform("cameraMatrix"), 1, GL_FALSE, glm::value_ptr(toSSpace * toView));
	glUniform1f(shader.uniform("nearPlane"), (GLfloat)nearPlane);
	glUniform1f(shader.uniform("farPlane"), (GLfloat)farPlane);
	glUniform3f(shader.uniform("cameraPosition"), position.x, position.y, position.z);
	glUniform3f(shader.uniform("cameraDirection"), forward.x, forward.y, forward.z);
	glUniform3f(shader.uniform("cameraUp"), up.x, up.y, up.z);
	glUniform1f(shader.uniform("cameraFOV"), fov);
}

void Camera::operateInputs(GLFWwindow* window, double deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position += glm::normalize(forward) * (float)(speed * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position -= glm::normalize(forward) * (float)(speed * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position -= glm::normalize(glm::cross(forward, up)) * (float)(speed * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position += glm::normalize(glm::cross(forward, up)) * (float)(speed * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		position -= glm::normalize(up) * (float)(speed * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		position += glm::normalize(up) * (float)(speed * deltaTime);
	}
	if (enabledRotation) {
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		glm::vec3 sideVector = glm::normalize(glm::cross(up, forward));
		float pitch = glm::radians(mouseY - displayHeight / 2.0) * sencitivity;
		float yaw = -glm::radians(mouseX - displayWidth / 2.0) * sencitivity;
		forward = glm::rotate(forward, yaw, up);
		glm::vec3 pitched = glm::rotate(forward, pitch, sideVector);
		if (true || (glm::angle(pitched, up) > glm::radians(5.0) && glm::angle(pitched, -up) > glm::radians(5.0))) {
			forward = pitched;
		}
		glfwSetCursorPos(window, displayWidth / 2.0, displayHeight / 2.0);
	}
	if (glfwGetMouseButton(window, 1) == GLFW_PRESS) {
		enabledRotation = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		glfwSetCursorPos(window, displayWidth / 2.0, displayHeight / 2.0);
	}
	if (glfwGetMouseButton(window, 1) == GLFW_RELEASE) {
		enabledRotation = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}