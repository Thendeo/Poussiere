/*----------------------------------------------------
**
**		Date: 2019
**		Author: Thendeo
**		Project: Particules
**		File: Particule header file
**
----------------------------------------------------*/

#pragma once

#include "Shader.h"

#include "Image.h"
#include "TextureDefinitions.h"


//! @brief Shader to draw particules
//! Allows one to draw multiple particules using a texture position and geometry shader
//! Vertex shader input : 
//! * MVP matrix for camera position
//! * Texture for each particule position
//! Geometry shader input :
//! * None
//! Fragment shader input :
//! * Texture for particule rendering
class ParticuleShader: public Shader
{
public:

	ParticuleShader(unsigned int pNumberOfParticule);
	~ParticuleShader();

	virtual void draw();

	void setVertexMVP(glm::mat4 pMVP);

private:
	
	// Uniform location
	GLuint m_UniformMVP;
	GLuint m_UniformTexturePosition;
	GLuint m_UniformTextureRendering;

	// Shader properties
	unsigned int m_ParticuleNumber;
};