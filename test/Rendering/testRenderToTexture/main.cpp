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
	glClearColor(0.1f, 0.1f, 0.8f, 0.0f);

	// The texture we're going to render to
	GLuint renderedTexture;
	glGenTextures(1, &renderedTexture);
	glActiveTexture(GL_TEXTURE0);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	// Give an empty image to OpenGL ( the last "0" means "empty" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	// Load initial position texture (or create from scratch)
	GLuint l_PositionTextureID = 0;
	glGenTextures(1, &l_PositionTextureID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, l_PositionTextureID);

	Image_8 l_PositionTexture;
	l_PositionTexture.loadFromPNG("position.png");

	GLint l_Type = 0;
	switch (l_PositionTexture.getImgType())
	{
	case ImageType::ImageType_RGB:
		l_Type = GL_RGB;
		break;
	case ImageType::ImageType_RGBA:
		l_Type = GL_RGBA;
		break;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, l_Type, l_PositionTexture.getWidth(), l_PositionTexture.getHeight(),
		0, l_Type, GL_UNSIGNED_BYTE, l_PositionTexture.getData());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// Load velocity texture
	GLuint l_VelocityTextureID = 0;
	glGenTextures(1, &l_VelocityTextureID);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, l_VelocityTextureID);


	Image_8 l_VelocityTexture;
	l_VelocityTexture.loadFromPNG("velocity.png");

	l_Type = 0;
	switch (l_VelocityTexture.getImgType())
	{
	case ImageType::ImageType_RGB:
		l_Type = GL_RGB;
		break;
	case ImageType::ImageType_RGBA:
		l_Type = GL_RGBA;
		break;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, l_Type, l_VelocityTexture.getWidth(), l_VelocityTexture.getHeight(),
		0, l_Type, GL_UNSIGNED_BYTE, l_VelocityTexture.getData());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// Create shader and loads paramaters
	GLuint l_AdditionProgramID = ShaderLoader::_loadShader("TexturedTriangle.vertexshader", "TexturedTriangle.geometryshader", "TexturedTriangle.fragmentshader");

	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	GLenum l_Err = glGetError();
	doAssert(l_Err == GL_NO_ERROR);

	// Render it
	glUseProgram(l_AdditionProgramID);
	GLuint l_TexturePositionLocation = glGetUniformLocation(l_AdditionProgramID, "TextureA");
	GLuint l_TexturePositionLocationBIS = glGetUniformLocation(l_AdditionProgramID, "myTextureSampler");
	GLuint l_TextureVelocityLocation = glGetUniformLocation(l_AdditionProgramID, "TextureB");
	glUniform1i(l_TexturePositionLocation, 1);
	glUniform1i(l_TextureVelocityLocation, 2);
	glUniform1i(l_TexturePositionLocationBIS, 2);
	

	GLuint l_vertexArray = 0;
	glGenVertexArrays(1, &l_vertexArray);
	glBindVertexArray(l_vertexArray);
	l_Err = glGetError();
	doAssert(l_Err == GL_NO_ERROR);


	// Save resulting image as test validation
	// ! Temp rendering to triangle

	GLuint l_TriangleProgramID = ShaderLoader::_loadShader("TexturedTriangle.vertexshader", "TexturedTriangle.geometryshader", "TexturedTriangle.fragmentshader");

	glUseProgram(l_TriangleProgramID);
	GLuint l_TextureLocation = glGetUniformLocation(l_TriangleProgramID, "myTextureSampler");
	glUniform1i(l_TextureLocation, 0);

	l_Err = glGetError();
	doAssert(l_Err == GL_NO_ERROR);

	bool l_shouldExit = false;
	do
	{
		glClearColor(0.8f, 0.1f, 0.1f, 0.0f);
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		glViewport(0, 0, 1024, 1024); // Render on the whole framebuffer, complete from the lower left corner to the upper right

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(l_AdditionProgramID);
		glDrawArrays(GL_POINTS, 0, 1);
		l_Err = glGetError();
		doAssert(l_Err == GL_NO_ERROR);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		l_Err = glGetError();
		doAssert(l_Err == GL_NO_ERROR);
		// Render on the whole framebuffer, complete from the lower left corner to the upper right
		glViewport(0, 0, 1728, 972);

		l_Err = glGetError();
		doAssert(l_Err == GL_NO_ERROR);

		glClearColor(0.1f, 0.1f, 0.8f, 0.0f);

		glUseProgram(l_TriangleProgramID);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_POINTS, 0, 1);
		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
			glfwWindowShouldClose(window) != 0)
		{
			l_shouldExit = true;
		}
		l_Err = glGetError();
		doAssert(l_Err == GL_NO_ERROR);
	} while (false == l_shouldExit);


	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.1f, 0.1f, 0.8f, 0.0f);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

