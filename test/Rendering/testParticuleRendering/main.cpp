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
#include "AssertHdl.h"
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
	window = glfwCreateWindow(1024, 768, "Tutorial 01", NULL, NULL);
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
	GLuint l_programID = ShaderLoader::_loadShader("DrawParticules.vertexshader", "DrawParticules.geometryshader", "DrawParticules.fragmentshader");

	GLuint l_vertexArray = 0;
	glGenVertexArrays(1, &l_vertexArray);
	glBindVertexArray(l_vertexArray);

	// Texture generation with random filled positions
	GLuint l_ParticulePositionTextureID = 0;
	glGenTextures(1, &l_ParticulePositionTextureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, l_ParticulePositionTextureID);
	

	Image_8 l_ParticulePositionTexture;
	l_ParticulePositionTexture.loadFromPNG("position.png");
	const size_t l_matrixSize = 4096;

	GLint l_Type = 0;
	switch (l_ParticulePositionTexture.getImgType())
	{
	case ImageType::ImageType_RGB:
		l_Type = GL_RGB;
		break;
	case ImageType::ImageType_RGBA:
		l_Type = GL_RGBA;
		break;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, l_Type, l_ParticulePositionTexture.getWidth(), l_ParticulePositionTexture.getHeight(),
		0, l_Type, GL_UNSIGNED_BYTE, l_ParticulePositionTexture.getData());
	/*
	unsigned char l_PrecomputedData[l_matrixSize * l_matrixSize * 3];
	for (int i_YIterator = 0; i_YIterator < l_matrixSize; i_YIterator++)
	{
		for (int i_XIterator = 0; i_XIterator < l_matrixSize; i_XIterator++)
		{
			for (int i_RGBIterator = 0; i_RGBIterator < 3U; i_RGBIterator++)
			{
				l_PrecomputedData[i_YIterator * l_matrixSize * 3U + i_XIterator * 3U + i_RGBIterator] = (rand() % 255);
			}
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, l_matrixSize, l_matrixSize, 0, GL_RGB, GL_UNSIGNED_BYTE, &l_PrecomputedData[0]);*/

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Texture loading for particule rendering
	GLuint l_ParticuleRenderingTextureID = 0;
	glGenTextures(1, &l_ParticuleRenderingTextureID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, l_ParticuleRenderingTextureID);

	Image_8 l_ParticuleRenderingTexture;
	l_ParticuleRenderingTexture.loadFromPNG("particule.png");

	l_Type = 0;
	switch (l_ParticuleRenderingTexture.getImgType())
	{
	case ImageType::ImageType_RGB:
		l_Type = GL_RGB;
		break;
	case ImageType::ImageType_RGBA:
		l_Type = GL_RGBA;
		break;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, l_Type, l_ParticuleRenderingTexture.getWidth(), l_ParticuleRenderingTexture.getHeight(),
		0, l_Type, GL_UNSIGNED_BYTE, l_ParticuleRenderingTexture.getData());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	
	/****************** MVP ******************/
	// Matrice de la caméra
	glm::mat4 View;
	// Matrice de projection : Champ de vision de 45° , ration 4:3, distance d'affichage : 0.1 unités <-> 100 unités 
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 500.0f);
	glm::mat4 MVP;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glUseProgram(l_programID);

	GLuint l_MatrixUniform = glGetUniformLocation(l_programID, "MVP");
	GLuint l_TextureUniform = glGetUniformLocation(l_programID, "particulePositionMatrix");
	GLuint l_TextureUniformFragment = glGetUniformLocation(l_programID, "particuleSampler");

	glUniform1i(l_TextureUniform, 0);
	glUniform1i(l_TextureUniformFragment, 1);

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
		glUniformMatrix4fv(l_MatrixUniform, 1, GL_FALSE, &MVP[0][0]);
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_POINTS, 0, l_matrixSize* l_matrixSize); // 12*3 indices starting at 0 -> 12 triangles

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

