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

#include <ParticuleShader.h>
#include "Image_8.h"

int main(void)
{
	// TODO move this to an openGLScene singleton
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
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	/****************** DATA ******************/

	GLfloat g_vertex_buffer_data[] = {
		0.0f, 0.0f, 0.0f,
		 1.0f, 0.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};

	GLfloat g_uv_buffer_data[] = {
		0.0, 0.0f,
		 2.0F, 0.0f,
		 0.0f,  2.0F
	};
		
	unsigned int l_TextureWidth = 2, l_TextureHeight = 2;
	unsigned char l_TextureData[16] = { 255, 0, 0,
		0, 255, 0,
		0, 0, 255,
		255, 255, 255 };


	/****************** SHADER ******************/ 
	ParticuleShader m_shader(g_vertex_buffer_data, sizeof(g_vertex_buffer_data), g_uv_buffer_data, sizeof(g_uv_buffer_data));
	Image_8 l_Tex;
	l_Tex.loadFromBMP("texture.bmp");
	m_shader.loadTexture(&l_Tex);

	/****************** MVP ******************/

	// TODO move this inside a camera object with configuration
	// Matrice de la caméra
	glm::mat4 View;
	//glm::mat4 Model = glm::mat4(1.0f);
	// Matrice de projection : Champ de vision de 45° , ration 4:3, distance d'affichage : 0.1 unités <-> 100 unités 
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 MVP;

	float base = 0.0;
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// 5 Particules + direction
	glm::mat4 Model[5];
	glm::vec3 ModelTranslation[5] = 
	{ 
		glm::vec3(1, 0, 0)
		, glm::vec3(0, 1, 0)
		, glm::vec3(-1, 0, 0)
		, glm::vec3(0, -1, 0)
		, glm::vec3(1, 1, 0)
	};

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// TODO replace those redondant lines by a texture with appropriate fragment code

		// Matrice de la caméra
		View = glm::lookAt(
			glm::vec3(1, 1, 1), // Position
			glm::vec3(0, 0, 0), // LookAt
			glm::vec3(0, 1, 0)  // Up vector
		);

		for (unsigned int l_ModelID = 0; l_ModelID < 5; l_ModelID++)
		{
			Model[l_ModelID] = glm::translate(ModelTranslation[l_ModelID] * base);
			MVP = Projection * View * Model[l_ModelID];
			m_shader.setVertexParameters(MVP);
			m_shader.draw();
		}
		
		// TODO move this awfull clock inside a real Timer with proper scheduler
		base += 0.002F;

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

