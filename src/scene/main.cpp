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
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	
	// Create vertex buffer data
	GLfloat g_vertex_buffer_data[] = {
		0.0f, 0.0f, 0.0f,
		 1.0f, 0.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};

	GLuint l_VertexBufferID1 = 0;
	glGenBuffers(1, &l_VertexBufferID1);
	glBindBuffer(GL_ARRAY_BUFFER, l_VertexBufferID1);
	glBufferData(GL_ARRAY_BUFFER, 36, g_vertex_buffer_data, GL_STATIC_DRAW);

	// Outside of the loop variables
	ParticuleShader m_shader;

	// Matrice de la caméra
	glm::mat4 View;
	// Matrice de modèle : une matrice d'identité (le modèle sera à l'origine) 
	glm::mat4 Model = glm::mat4(1.0f);  // Changez pour chaque modèle ! 
	// Matrice de projection : Champ de vision de 45° , ration 4:3, distance d'affichage : 0.1 unités <-> 100 unités 
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 MVP;

	float base = 0.0;

	do {

		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClear(GL_COLOR_BUFFER_BIT);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Matrice de la caméra
		View = glm::lookAt(
			glm::vec3(4, 3, 3), // La caméra est à (4,3,3), dans l'espace monde
			glm::vec3(0, -base, 0), // et regarde l'origine
			glm::vec3(0, 1, 0)  // La tête est vers le haut (utilisez 0,-1,0 pour regarder à l'envers) 
		);
		MVP = Projection * View * Model; // Souvenez-vous, la multiplication de matrice fonctionne dans l'autre sens
		m_shader.setVertexParameters(MVP);

		m_shader.bind(l_VertexBufferID1);
		m_shader.draw();

		// Matrice de la caméra
		View = glm::lookAt(
			glm::vec3(4, 3, 3), // La caméra est à (4,3,3), dans l'espace monde
			glm::vec3(0, base, 0), // et regarde l'origine
			glm::vec3(0, 1, 0)  // La tête est vers le haut (utilisez 0,-1,0 pour regarder à l'envers) 
		);
		// Notre matrice ModelViewProjection : la multiplication des trois  matrices 
		MVP = Projection * View * Model; // Souvenez-vous, la multiplication de matrice fonctionne dans l'autre sens

		m_shader.setVertexParameters(MVP);
		m_shader.bind(l_VertexBufferID1);
		m_shader.draw();

		// Matrice de la caméra
		View = glm::lookAt(
			glm::vec3(4, 3, 3), // La caméra est à (4,3,3), dans l'espace monde
			glm::vec3(base, 0, 0), // et regarde l'origine
			glm::vec3(0, 1, 0)  // La tête est vers le haut (utilisez 0,-1,0 pour regarder à l'envers) 
		);
		// Notre matrice ModelViewProjection : la multiplication des trois  matrices 
		MVP = Projection * View * Model; // Souvenez-vous, la multiplication de matrice fonctionne dans l'autre sens

		m_shader.setVertexParameters(MVP);
		m_shader.bind(l_VertexBufferID1);
		m_shader.draw();

		// Matrice de la caméra
		View = glm::lookAt(
			glm::vec3(4, 3, 3), // La caméra est à (4,3,3), dans l'espace monde
			glm::vec3(-base, 0, 0), // et regarde l'origine
			glm::vec3(0, 1, 0)  // La tête est vers le haut (utilisez 0,-1,0 pour regarder à l'envers) 
		);
		// Notre matrice ModelViewProjection : la multiplication des trois  matrices 
		MVP = Projection * View * Model; // Souvenez-vous, la multiplication de matrice fonctionne dans l'autre sens

		m_shader.setVertexParameters(MVP);
		m_shader.bind(l_VertexBufferID1);
		m_shader.draw();

		// Matrice de la caméra
		View = glm::lookAt(
			glm::vec3(4, 3, 3), // La caméra est à (4,3,3), dans l'espace monde
			glm::vec3(0, 0, base), // et regarde l'origine
			glm::vec3(0, 1, 0)  // La tête est vers le haut (utilisez 0,-1,0 pour regarder à l'envers) 
		);
		// Notre matrice ModelViewProjection : la multiplication des trois  matrices 
		MVP = Projection * View * Model; // Souvenez-vous, la multiplication de matrice fonctionne dans l'autre sens

		m_shader.setVertexParameters(MVP);
		m_shader.bind(l_VertexBufferID1);
		m_shader.draw();

		// Matrice de la caméra
		View = glm::lookAt(
			glm::vec3(4, 3, 3), // La caméra est à (4,3,3), dans l'espace monde
			glm::vec3(0, 0, -base), // et regarde l'origine
			glm::vec3(0, 1, 0)  // La tête est vers le haut (utilisez 0,-1,0 pour regarder à l'envers) 
		);
		// Notre matrice ModelViewProjection : la multiplication des trois  matrices 
		MVP = Projection * View * Model; // Souvenez-vous, la multiplication de matrice fonctionne dans l'autre sens

		m_shader.setVertexParameters(MVP);
		m_shader.bind(l_VertexBufferID1);
		m_shader.draw();
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

