
cmake_minimum_required(VERSION 3.18) # b/c of use of BUILD_INTERFACE generator expression

project(Particules)

# Packages definitions
find_package(PNG 1.6.31)
find_package(TIFF 4.1.0)
find_package(OpenGL 3.3)
find_package(GLEW 2.1.0)
find_path(GLM_INCLUDE_DIRS glm/)
find_path(GLFW_INCLUDE_DIRS glfw/)
find_library(GLFW_LIB_DEBUG glfw/)
find_library(GLFW_LIB_RELEASE glfw/)

set(GLFW_LIBS "optimized" ${GLFW_LIB_DEBUG} ${GLFW_LIB_RELEASE})

# Set variables for sources and headers
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
	src/Renderer/UpdateVelocity.h
	src/Renderer/Shader.h)
	
set(Renderer_SOURCES
	src/Renderer/AdvanceShader.cpp
	src/Renderer/ParticuleShader.cpp
	src/Renderer/UpdateVelocity.cpp
	src/Renderer/Shader.cpp)
	
set(Shader_EXTERNAL
	src/Shaders/DrawParticules.vertexshader
	src/Shaders/DrawParticules.geometryshader
	src/Shaders/DrawParticules.fragmentshader
	src/Shaders/TextureAddition.fragmentshader
	src/Shaders/DynamicDecreaseVelocity.fragmentshader
	src/Shaders/TexturedTriangle.vertexshader
	src/Shaders/TexturedTriangle.geometryshader
	src/Shaders/TexturedTriangle.fragmentshader)
	
# Conveniance variable for all files
set(All_SOURCES  ${Image_SOURCES} ${Texture_SOURCES} ${Platform_SOURCES} ${Renderer_SOURCES}
				${Image_HEADERS} ${Texture_HEADERS} ${Platform_HEADERS} ${Renderer_HEADERS}
				${Shader_EXTERNAL} )

# Prepare subfolder for sources classification
source_group("Sources/Data/Image"	FILES ${Image_SOURCES})
source_group("Sources/Data/Texture"	FILES ${Texture_SOURCES})
source_group("Sources/Platform"	FILES ${Platform_SOURCES})
source_group("Sources/Renderer"	FILES ${Renderer_SOURCES})

source_group("Headers/Data/Image"	FILES ${Image_HEADERS})
source_group("Headers/Data/Texture"	FILES ${Texture_HEADERS})
source_group("Headers/Platform"	FILES ${Platform_HEADERS} )
source_group("Headers/Renderer"	FILES ${Renderer_HEADERS})

source_group("Shaders"	FILES ${Shader_EXTERNAL})




# Tests definitions
add_executable(testRenderToTexture test/Rendering/testRenderToTexture/main.cpp ${All_SOURCES})
source_group("Sources"	FILES test/Rendering/testRenderToTexture/main.cpp)
add_executable(testParticuleRendering test/Rendering/testParticuleRendering/main.cpp ${All_SOURCES})
source_group("Sources"	FILES test/Rendering/testParticuleRendering/main.cpp)
add_executable(testConstantVelocity test/Rendering/testConstantVelocity/main.cpp ${All_SOURCES})
source_group("Sources"	FILES test/Rendering/testConstantVelocity/main.cpp)
add_executable(testDynamicVelocity test/Rendering/testDynamicVelocity/main.cpp ${All_SOURCES})
source_group("Sources"	FILES test/Rendering/testDynamicVelocity/main.cpp)
add_executable(testDecreaseParticule test/Rendering/testDecreaseParticule/main.cpp ${All_SOURCES})
source_group("Sources"	FILES test/Rendering/testDecreaseParticule/main.cpp)


											
target_include_directories(testRenderToTexture PUBLIC
							src/Data/Image
							src/Data/Texture
							src/Platform
							src/Renderer
							${PNG_INCLUDE_DIRS}
							${TIFF_INCLUDE_DIR}
							${OPENGL_INCLUDE_DIR}
							${GLEW_INCLUDE_DIRS}
							${GLM_INCLUDE_DIRS}
							${GLFW_INCLUDE_DIRS}
                           )
target_include_directories(testParticuleRendering PUBLIC
							src/Data/Image
							src/Data/Texture
							src/Platform
							src/Renderer
							${PNG_INCLUDE_DIRS}
							${TIFF_INCLUDE_DIR}
							${OPENGL_INCLUDE_DIR}
							${GLEW_INCLUDE_DIRS}
							${GLM_INCLUDE_DIRS}
							${GLFW_INCLUDE_DIRS}
                           )
target_include_directories(testConstantVelocity PUBLIC
							src/Data/Image
							src/Data/Texture
							src/Platform
							src/Renderer
							${PNG_INCLUDE_DIRS}
							${TIFF_INCLUDE_DIR}
							${OPENGL_INCLUDE_DIR}
							${GLEW_INCLUDE_DIRS}
							${GLM_INCLUDE_DIRS}
							${GLFW_INCLUDE_DIRS}
                           )
target_include_directories(testDynamicVelocity PUBLIC
							src/Data/Image
							src/Data/Texture
							src/Platform
							src/Renderer
							${PNG_INCLUDE_DIRS}
							${TIFF_INCLUDE_DIR}
							${OPENGL_INCLUDE_DIR}
							${GLEW_INCLUDE_DIRS}
							${GLM_INCLUDE_DIRS}
							${GLFW_INCLUDE_DIRS}
                           )
target_include_directories(testDecreaseParticule PUBLIC
							src/Data/Image
							src/Data/Texture
							src/Platform
							src/Renderer
							${PNG_INCLUDE_DIRS}
							${TIFF_INCLUDE_DIR}
							${OPENGL_INCLUDE_DIR}
							${GLEW_INCLUDE_DIRS}
							${GLM_INCLUDE_DIRS}
							${GLFW_INCLUDE_DIRS}
                           )
						   
target_link_libraries(testRenderToTexture ${PNG_LIBRARIES} ${TIFF_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBS})
target_link_libraries(testParticuleRendering ${PNG_LIBRARIES} ${TIFF_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBS})
target_link_libraries(testConstantVelocity ${PNG_LIBRARIES} ${TIFF_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBS})
target_link_libraries(testDynamicVelocity ${PNG_LIBRARIES} ${TIFF_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBS})
target_link_libraries(testDecreaseParticule ${PNG_LIBRARIES} ${TIFF_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBS})