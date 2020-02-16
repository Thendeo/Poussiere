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

// TODO : move this to an image loader
GLuint loadBMP_custom(const char* imagepath, unsigned char** outData, unsigned int &outWidth, unsigned int &outHeight)
{
	// Données lues à partir de l'en-tête du fichier BMP
	unsigned char header[54]; // Chaque fichier BMP débute par un en-tête de 54 octets
	unsigned int dataPos;     // Position dans le fichier où les données débutent
	unsigned int imageSize;   // = width*height*3 
	// les données RBG

	FILE* file = fopen(imagepath, "rb");
	if (!file) { printf("Image could not be opened\n"); return 0; }

	// Header
	if (fread(header, 1, 54, file) != 54)
	{ // S'il n'est pas possible de lire 54 octets : problème
		printf("Not a correct BMP file\n");
		return false;
	}

	// Verif
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return 0;
	}

	// Read size
	// Lit des entiers à partir du tableau d'octets
	dataPos = *(int*) & (header[0x0A]);
	imageSize = *(int*) & (header[0x22]) * 3;
	outWidth = *(int*) & (header[0x12]);
	outHeight = *(int*) & (header[0x16]);

	// Bad sizing / anomalies
	// Certains fichiers BMP sont mal formés, on devine les informations manquantes
	if (imageSize == 0)    imageSize = outWidth * outHeight * 3; // 3 : un octet pour chaque composante rouge, vert et bleu
	if (dataPos == 0)      dataPos = 54; // l'en-tête BMP est fait de cette façon

	// Buffer allocation
	// Crée un tampon
	*outData = (unsigned char*)malloc(imageSize);

	// Lit les données à partir du fichier pour les mettre dans le tampon
	fread(*outData, 1, imageSize, file);

	// Tout est en mémoire maintenant, le fichier peut être fermé
	fclose(file);

}

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
		 1.0F, 0.0f,
		 0.0f,  1.0F
	};
		
	unsigned int l_TextureWidth = 2, l_TextureHeight = 2;
	unsigned char l_TextureData[16] = { 255, 0, 0,
		0, 255, 0,
		0, 0, 255,
		255, 255, 255 };


	/****************** SHADER ******************/ 
	ParticuleShader m_shader(g_vertex_buffer_data, sizeof(g_vertex_buffer_data), g_uv_buffer_data, sizeof(g_uv_buffer_data));
	m_shader.loadTexture(l_TextureWidth, l_TextureHeight, l_TextureData);

	/****************** MVP ******************/

	// TODO move this inside a camera object with configuration
	// Matrice de la caméra
	glm::mat4 View;
	glm::mat4 Model = glm::mat4(1.0f);
	// Matrice de projection : Champ de vision de 45° , ration 4:3, distance d'affichage : 0.1 unités <-> 100 unités 
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 MVP;

	float base = 0.0;
	// Active le test de profondeur
	glEnable(GL_DEPTH_TEST);
	// Accepte le fragment s'il est plus proche de la caméra que le précédent accepté
	glDepthFunc(GL_LESS);

	// Loop that draws several triangles based on a single vertex array and multiple MVP changes
	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// TODO replace those redondant lines by a texture with appropriate fragment code

		// Matrice de la caméra
		View = glm::lookAt(
			glm::vec3(4, 3, 3), // La caméra est à (4,3,3), dans l'espace monde
			glm::vec3(0, -base, 1), // et regarde l'origine
			glm::vec3(0, 1, 0)  // La tête est vers le haut (utilisez 0,-1,0 pour regarder à l'envers) 
		);
		MVP = Projection * View * Model; 
		m_shader.setVertexParameters(MVP);

		m_shader.draw();

		View = glm::lookAt(
			glm::vec3(4, 3, 3),
			glm::vec3(0, base, 0),
			glm::vec3(0, 1, 0)
		);
		MVP = Projection * View * Model;

		m_shader.setVertexParameters(MVP);
		m_shader.draw();

		View = glm::lookAt(
			glm::vec3(4, 3, 3), 
			glm::vec3(base, 0, 1),
			glm::vec3(0, 1, 0)
		);
		MVP = Projection * View * Model;

		m_shader.setVertexParameters(MVP);
		m_shader.draw();

		View = glm::lookAt(
			glm::vec3(4, 3, 3),
			glm::vec3(-base, 0, 0),
			glm::vec3(0, 1, 0)
		);
		MVP = Projection * View * Model;

		m_shader.setVertexParameters(MVP);
		m_shader.draw();

		View = glm::lookAt(
			glm::vec3(4, 3, 3), 
			glm::vec3(0, 0, base),
			glm::vec3(0, 1, 0)
		);
		MVP = Projection * View * Model;

		m_shader.setVertexParameters(MVP);
		m_shader.draw();

		View = glm::lookAt(
			glm::vec3(4, 3, 3),
			glm::vec3(0, 0, -base), 
			glm::vec3(0, 1, 0)
		);
		MVP = Projection * View * Model;

		m_shader.setVertexParameters(MVP);
		m_shader.draw();

		// TODO move this awfull clock inside a real MasterTimer with proper scheduler
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

