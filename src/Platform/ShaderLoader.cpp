/*----------------------------------------------------
**
**		Date: 2019
**		Author: Thendeo
**		Project: Particules
**		File: ShaderLoader implementation file
**
----------------------------------------------------*/
#include "ShaderLoader.h"

#include "AssertHdl.h"

GLuint ShaderLoader::_loadShader(const char* pVertexFilePath, const char* pFragmentFilePath)
{
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	loadVertexShader(pVertexFilePath, VertexShaderID);
	loadFragmentShader(pFragmentFilePath, FragmentShaderID);
	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

GLuint ShaderLoader::_loadShader(const char* pVertexFilePath, const char* pGeometryFilePath, const char* pFragmentFilePath)
{
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	std::string l_ShaderPath(getenv("ShaderLoc"));
	std::string l_VertexFullPath(l_ShaderPath + pVertexFilePath);
	std::string l_GeometryFullPath(l_ShaderPath + pGeometryFilePath);
	std::string l_FragmentFullPath(l_ShaderPath + pFragmentFilePath);

	loadVertexShader(l_VertexFullPath.c_str(), VertexShaderID);
	loadGeometryShader(l_GeometryFullPath.c_str(), GeometryShaderID);
	loadFragmentShader(l_FragmentFullPath.c_str(), FragmentShaderID);
	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, GeometryShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, GeometryShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(GeometryShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

void ShaderLoader::loadVertexShader(const char* pVertexFilePath, GLuint& pVertexShaderID)
{
	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(pVertexFilePath, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s", pVertexFilePath);
		getchar();
		doAssert(false);
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", pVertexFilePath);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(pVertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(pVertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(pVertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(pVertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(pVertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}
}

void ShaderLoader::loadGeometryShader(const char* pGeometryFilePath, GLuint& pGeometryShaderID)
{
	// Read the Geometry Shader code from the file
	std::string GeometryShaderCode;
	std::ifstream GeometryShaderStream(pGeometryFilePath, std::ios::in);
	if (GeometryShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << GeometryShaderStream.rdbuf();
		GeometryShaderCode = sstr.str();
		GeometryShaderStream.close();
	}
	// Compile Geometry Shader
	printf("Compiling shader : %s\n", pGeometryFilePath);
	char const* GeometrySourcePointer = GeometryShaderCode.c_str();
	glShaderSource(pGeometryShaderID, 1, &GeometrySourcePointer, NULL);
	glCompileShader(pGeometryShaderID);

	GLint Result = GL_FALSE;
	int InfoLogLength;
	// Check Geometry Shader
	glGetShaderiv(pGeometryShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(pGeometryShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> GeometryShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(pGeometryShaderID, InfoLogLength, NULL, &GeometryShaderErrorMessage[0]);
		printf("%s\n", &GeometryShaderErrorMessage[0]);
	}
}

void ShaderLoader::loadFragmentShader(const char* pFragmentFilePath, GLuint& pFragmentShaderID)
{

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(pFragmentFilePath, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", pFragmentFilePath);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(pFragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(pFragmentShaderID);

	GLint Result = GL_FALSE;
	int InfoLogLength;
	// Check Fragment Shader
	glGetShaderiv(pFragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(pFragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(pFragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}
}
