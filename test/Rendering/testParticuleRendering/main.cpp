// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp >
#include <glm/gtx/transform.hpp>

#include "Image_8.h"
#include "Texture2D.h"
#include "AssertHdl.h"
#include "ParticuleShader.h"
#include "ShaderLoader.h"

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1728, 972, "Tutorial 01", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true; // Needed for core profile
	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.1f, 0.1f, 0.2f, 0.0f);

	/****************** SHADER ******************/
	GLuint l_err = GL_NO_ERROR;

	GLuint l_vertexArray = 0;
	glGenVertexArrays(1, &l_vertexArray);
	glBindVertexArray(l_vertexArray);


	const size_t l_matrixSize = 4096;
	Texture2D l_ParticulePosition("position.png", eTextureUnitMap::eTUM_Position);
	Texture2D l_ParticuleRendering("particule.png", eTextureUnitMap::eTUM_ParticuleTexture);

	
	/****************** MVP ******************/
	// Matrice de la caméra
	glm::mat4 View;
	// Matrice de projection : Champ de vision de 45° , ration 4:3, distance d'affichage : 0.1 unités <-> 100 unités 
	glm::mat4 Projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 500.0f);
	glm::mat4 MVP;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	ParticuleShader l_ParticuleShader(l_matrixSize * l_matrixSize);

	l_err = glGetError();

	if (l_err != GL_NO_ERROR)
	{
		std::cout << "EError before loop ! " << l_err << std::endl;
		l_err = GL_NO_ERROR;
	}


	float base = 0.0;
	bool l_shouldExit = false;
	do 
	{
		View = glm::lookAt(
			glm::vec3(200 * std::cos(base), 0, 200 * std::sin(base)), // Position
			glm::vec3(0, 0, 0), // LookAt
			glm::vec3(0, 1, 0)  // Up vector
		);
		MVP = Projection * View;

		l_ParticuleShader.setVertexMVP(MVP);

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		l_ParticuleShader.draw();

		// TODO move this awfull clock inside a real Timer with proper scheduler
		base += 0.00016F;

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		l_err = glGetError();

		if (l_err != GL_NO_ERROR)
		{
			std::cout << "Error during loop ! " << l_err << std::endl;
			l_err = GL_NO_ERROR;
			l_shouldExit = true;
		}


		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
			glfwWindowShouldClose(window) != 0)
		{
			l_shouldExit = true;
		}
	} // Check if the ESC key was pressed or the window was closed
	while (false == l_shouldExit);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

