#include<iostream>
#include<vector>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "StaticMesh.h"
#include "Camera.h"

int main() {
	glfwInit();

	glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "Main app", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to initialize window, terminating" << std::endl;
		glfwTerminate();
		return 0;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, 800, 800);

	Shader opaqueShader("final.vert", "brdf.frag");

	Camera camera = Camera(CAMERA_PERSPECTIVE);

	StaticMesh cube;
	cube.loadFile("CubeMeshSmooth.txt");
	cube.updateBuffers();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	float time = glfwGetTime();
	int tick = 0;
	while (!glfwWindowShouldClose(window)) {
		float newTime = glfwGetTime();
		float deltaTime = newTime - time;
		time = newTime;
		tick++;

		glfwSetWindowTitle(window, ("Game window, FPS:" + std::to_string(1.0 / deltaTime)).c_str());

		glClearColor(0.1, 0.2, 0.4, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (tick % 100 == 0) {
			std::cout << "Camera position: " << camera.position.x << ' ' << 
				camera.position.y << ' ' << camera.position.z << std::endl;
		}

		cube.draw(camera, opaqueShader);

		glfwSwapBuffers(window);

		camera.operateInputs(window, deltaTime);
		glfwPollEvents();
	}

	glfwTerminate();
}