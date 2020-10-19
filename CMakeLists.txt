
cmake_minimum_required(VERSION 3.18) # b/c of use of BUILD_INTERFACE generator expression

project(Particules)

find_package(PNG 1.6.31)
find_package(TIFF 4.1.0)
find_package(OpenGL 3.3)
find_package(GLEW 2.1.0)

set(Image_HEADERS
	src/Data/Image/Image.h
	src/Data/Image/ImageType.h)
	
set(Image_SOURCES
	src/Data/Image/Image.cpp)
	
set(Texture_HEADERS
	src/Data/Texture/Texture2D.h
	src/Data/Texture/TextureDefinitions.h)
	
set(Texture_SOURCES
	src/Data/Texture/Texture2D.cpp)
	
set(Platform_HEADERS
	src/Platform/AssertHdl.h
	src/Platform/ShaderLoader.h)
	
set(Platform_SOURCES
	src/Platform/AssertHdl.cpp
	src/Platform/ShaderLoader.cpp)
	
set(Renderer_HEADERS
	src/Renderer/AdvanceShader.h
	src/Renderer/ParticuleShader.h
	src/Renderer/Shader.h)
	
set(Renderer_SOURCES
	src/Renderer/AdvanceShader.cpp
	src/Renderer/ParticuleShader.cpp
	src/Renderer/Shader.cpp)
	
set(Shader_EXTERNAL
	src/Shaders/DrawParticules.vertexshader
	src/Shaders/DrawParticules.geometryshader
	src/Shaders/DrawParticules.fragmentshader
	src/Shaders/TextureAddition.fragmentshader
	src/Shaders/TexturedTriangle.vertexshader
	src/Shaders/TexturedTriangle.geometryshader
	src/Shaders/TexturedTriangle.fragmentshader)

source_group("Sources/Data/Image"	FILES ${Image_SOURCES})
source_group("Sources/Data/Texture"	FILES ${Texture_SOURCES})
source_group("Sources/Platform"	FILES ${Platform_SOURCES})
source_group("Sources/Renderer"	FILES ${Renderer_SOURCES})

source_group("Headers/Data/Image"	FILES ${Image_HEADERS})
source_group("Headers/Data/Texture"	FILES ${Texture_HEADERS})
source_group("Headers/Platform"	FILES ${Platform_HEADERS} )
source_group("Headers/Renderer"	FILES ${Renderer_HEADERS})

source_group("Shaders"	FILES ${Shader_EXTERNAL})

source_group("Sources"	FILES test/Rendering/testRenderToTexture/main.cpp)


add_executable(testRenderToTexture test/Rendering/testRenderToTexture/main.cpp ${Image_SOURCES} ${Texture_SOURCES} ${Platform_SOURCES} ${Renderer_SOURCES}
											${Image_HEADERS} ${Texture_HEADERS} ${Platform_HEADERS} ${Renderer_HEADERS}
											${Shader_EXTERNAL})
											
target_include_directories(testRenderToTexture PUBLIC
							src/Data/Image
							src/Data/Texture
							src/Platform
							src/Renderer
							${PNG_INCLUDE_DIRS}
							${TIFF_INCLUDE_DIR}
							${OPENGL_INCLUDE_DIR}
                           )
						   
target_link_libraries(testRenderToTexture ${PNG_LIBRARIES} ${TIFF_LIBRARIES} ${OPENGL_LIBRARIES})