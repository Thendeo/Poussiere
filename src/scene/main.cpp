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
#include "AdvanceShader.h"
#include "Image_8.h"
#include "AssertHdl.h"
#include "ShaderLoader.h"

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
	glClearColor(0.1f, 0.4f, 0.5f, 0.0f);

	/****************** DATA ******************/

	GLfloat g_vertex_buffer_data[] = {
		0.0f, 0.0f, 0.0f
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

	AdvanceShader m_AdvanceShader;

	Image_8 l_Tex;
	l_Tex.loadFromPNG("pos.png");
	//m_shader.loadTexture(&l_Tex);


	/* TEMPORARY TEXTURE LOADER START*/
	/*
	// Next step, replace this by additive rendertotexture
	GLint l_Type = 0;
	switch (l_Tex.getImgType())
	{
	case ImageType::ImageType_RGB:
		l_Type = GL_RGB;
		break;
	case ImageType::ImageType_RGBA:
		l_Type = GL_RGBA;
		break;
	}

	GLuint l_TextureParticule;
	glGenTextures(1, &l_TextureParticule);
	glBindTexture(GL_TEXTURE_2D, l_TextureParticule);

	glTexImage2D(GL_TEXTURE_2D, 0, l_Type, l_Tex.getWidth(), l_Tex.getHeight(), 0, l_Type, GL_UNSIGNED_BYTE, l_Tex.getData());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);*/
	
	
	Image_8 l_TexPos1;
	l_TexPos1.loadFromPNG("pos1.png");
	Image_8 l_TexPos2;
	l_TexPos2.loadFromPNG("pos2.png");

	// Create Arrays
	GLuint l_AdvanceVAO = 0;
	glGenVertexArrays(1, &l_AdvanceVAO);
	glBindVertexArray(l_AdvanceVAO);

	GLuint l_AdvanceProgramID = ShaderLoader::_loadShader("AdvancVertex.vertexshader",
		"SimpleGeometryShader.geometryshader", "AdvanceFragment.fragmentshader");



	GLuint l_TextureID1 = 0;
	glGenTextures(1, &l_TextureID1);
	glBindTexture(GL_TEXTURE_2D, l_TextureID1);

	GLint l_Type = 0;
	switch (l_TexPos1.getImgType())
	{
	case ImageType::ImageType_RGB:
		l_Type = GL_RGB;
		break;
	case ImageType::ImageType_RGBA:
		l_Type = GL_RGBA;
		break;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, l_Type, l_TexPos1.getWidth(), l_TexPos1.getHeight(), 0, l_Type, GL_UNSIGNED_BYTE, l_TexPos1.getData());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//l_TextureID1 = glGetUniformLocation(l_AdvanceProgramID, "advanceTexture");

	GLuint l_TextureID2 = 0;
	glGenTextures(1, &l_TextureID2);
	glBindTexture(GL_TEXTURE_2D, l_TextureID2);

	switch (l_TexPos2.getImgType())
	{
	case ImageType::ImageType_RGB:
		l_Type = GL_RGB;
		break;
	case ImageType::ImageType_RGBA:
		l_Type = GL_RGBA;
		break;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, l_Type, l_TexPos2.getWidth(), l_TexPos2.getHeight(), 0, l_Type, GL_UNSIGNED_BYTE, l_TexPos2.getData());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//l_TextureID2 = glGetUniformLocation(l_AdvanceProgramID, "posTexture");

	// Le tampon d'image, regroupant 0, 1 ou plus de textures et 0 et 1 tampon de profondeur. 
	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// La texture dans laquelle on va dessiner
	GLuint l_renderedTexture;
	glGenTextures(1, &l_renderedTexture);

	// "Lie" la nouvelle texture créée : toutes les fonctions suivantes vont modifier cette texture
	glBindTexture(GL_TEXTURE_2D, l_renderedTexture);

	// Donne une image vide à OpenGL (le dernier « 0 ») 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Filtrage léger. Obligatoire ! 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// Définit "renderedTexture" comme notre couleur d'attache #0 
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, l_renderedTexture, 0);

	// Définit la liste de tampons à dessiner. 
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" est la taille de DrawBuffers

	GLuint l_Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	doAssert(GL_FRAMEBUFFER_COMPLETE == l_Status);

	// Rendu dans le tampon d'image
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	glViewport(0, 0, 256, 256); // Rendu dans l'intégralité du tampon d'image, du coin inférieur gauche au coin supérieur droit

		// Use our shader
	glUseProgram(l_AdvanceProgramID);

	// Bind our texture in Texture Unit 0
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, l_TextureID1);

	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, l_TextureID2);

	glDrawArrays(GL_POINTS, 0, 3); // 12*3 indices starting at 0 -> 12 triangles*/
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 1024, 768);

	/* TEMPORARY TEXTURE LOADER END*/


	Image_8 l_PositionTex;
	l_PositionTex.loadFromBMP("pos.bmp");
//	m_AdvanceShader.setAdvanceTexture(&l_PositionTex);

	Image_8 l_AdvanceTex;
	l_AdvanceTex.loadFromBMP("advance.bmp");
//	m_AdvanceShader.setPositionTexture(&l_AdvanceTex);

	// Set vertex matrix from texture
	// Set result of advance input of particule
	// Set input of advance latest computed texture (swap)
	// Set velocity map from texture

	// load program

	/****************** MVP ******************/

	// TODO move this inside a camera object with configuration
	// Matrice de la caméra
	glm::mat4 View;
	//glm::mat4 Model = glm::mat4(1.0f);
	// Matrice de projection : Champ de vision de 45° , ration 4:3, distance d'affichage : 0.1 unités <-> 100 unités 
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 MVP;

	float base = 0.0;
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// 5 Particules + direction
	const unsigned int l_NbModel = 128;
	glm::mat4 Model[l_NbModel];

	glm::vec3 ModelTranslation[l_NbModel];
	for(unsigned int l_IdModel = 0; l_IdModel< l_NbModel; l_IdModel++)
	{
		ModelTranslation[l_IdModel] = glm::vec3((1 == (rand() % 2) ? -1 : 1) * (rand() % 100) / 100.0F,
			(1 == (rand() % 2) ? -1 : 1) * (rand() % 100) / 100.0F,
			(1 == (rand() % 2) ? -1 : 1) * (rand() % 100) / 100.0F);
	}

	// Matrice de la caméra
	View = glm::lookAt(
		glm::vec3(4, 1, 4), // Position
		glm::vec3(0, 0, 0), // LookAt
		glm::vec3(0, 1, 0)  // Up vector
	);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, l_renderedTexture);
	m_shader.setTexture(0);

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// TODO replace those redondant lines by a texture with appropriate fragment code


		for (unsigned int l_ModelID = 0; l_ModelID < l_NbModel; l_ModelID++)
		{
			Model[l_ModelID] = glm::translate(ModelTranslation[l_ModelID] * base);
			MVP = Projection * View * Model[l_ModelID];
			m_shader.setVertexParameters(MVP);
			m_shader.draw();
		}
		
		// TODO move this awfull clock inside a real Timer with proper scheduler
		base += 0.00016F;

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

