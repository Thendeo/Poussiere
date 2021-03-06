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

#include "Texture2D.h"
#include "AssertHdl.h"
#include "ParticuleShader.h"
#include "ShaderLoader.h"
#include "AdvanceShader.h"
#include "UpdateVelocity.h"
#include "Image.h"

#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

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
	glClearColor(0.1f, 0.1f, 0.8f, 0.0f);


	// Load initial position, rendered target and velocity
	Image<UWord> l_PositionData("position_origin.tif");
	Texture2D l_PositionTexture(&l_PositionData, eTextureUnitMap::eTUM_Position);

	Texture2D l_UpdatedPositionTexture(l_PositionTexture.getWidth(), l_PositionTexture.getHeight()
		, l_PositionTexture.getTextureType(), sizeof(UWord), eTextureUnitMap::eTUM_UpdatedPosition);
	Image<UWord> l_VelocityData("velocity_random.tif");
	Texture2D l_VelocityTexture(&l_VelocityData, eTextureUnitMap::eTUM_Velocity);
	Texture2D l_UpdatedVelocityTexture(l_VelocityTexture.getWidth(), l_VelocityTexture.getHeight()
		, l_VelocityTexture.getTextureType(), sizeof(UWord), eTextureUnitMap::eTUM_UpdatedVelocity);

	Image<UByte> l_RenderingImage("particule.png");
	Texture2D l_RenderingParticule(&l_RenderingImage, eTextureUnitMap::eTUM_ParticuleTexture);

	// Create shader and loads paramaters
	AdvanceShader l_AdditionShader(l_PositionTexture.getWidth());
	l_AdditionShader.setOutputLocation(l_PositionTexture.getTextureName(), l_UpdatedPositionTexture.getTextureName());

	UpdateVelocity l_GravShader(l_VelocityTexture.getWidth());
	l_GravShader.setOutputLocation(l_VelocityTexture.getTextureName(), l_UpdatedVelocityTexture.getTextureName());

	ParticuleShader l_RenderingShader(l_PositionTexture.getSize());

	GLenum l_Err = glGetError();
	doAssert(l_Err == GL_NO_ERROR);


	/****************** MVP ******************/
	// Matrice de la cam�ra
	glm::mat4 View;
	// Matrice de projection : Champ de vision de 45� , ration 4:3, distance d'affichage : 0.1 unit�s <-> 100 unit�s 
	glm::mat4 Projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 500.0f);
	glm::mat4 MVP;
	View = glm::lookAt(
		glm::vec3(120, 0, 120), // Position
		glm::vec3(0, 0, 0), // LookAt
		glm::vec3(0, 1, 0)  // Up vector
	);
	MVP = Projection * View;

	l_RenderingShader.setVertexMVP(MVP);

	// Render it
	GLuint l_vertexArray = 0;
	glGenVertexArrays(1, &l_vertexArray);
	glBindVertexArray(l_vertexArray);
	l_Err = glGetError();
	doAssert(l_Err == GL_NO_ERROR);

	bool l_shouldExit = false;
	unsigned int l_frameNb = 0;
	do
	{
		glClearColor(0.1f, 0.1f, 0.2f, 0.0f);
		l_GravShader.draw();

		l_AdditionShader.draw();


		l_RenderingShader.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
			glfwWindowShouldClose(window) != 0)
		{
			l_shouldExit = true;
		}
		l_Err = glGetError();
		doAssert(l_Err == GL_NO_ERROR);
		std::cout << "Frame : " << l_frameNb << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		l_frameNb++;
	} while (false == l_shouldExit);


	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.1f, 0.1f, 0.8f, 0.0f);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

