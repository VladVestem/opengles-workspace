#include <renderer.hpp>
#include <exception.hpp>

#include <memory>
#include <optional>
#include <cassert>
#include <array>

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <game_logic.hpp>

namespace opengles_workspace
{
	void LoadTexture(const char* path)
	{
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// set the texture wrapping/filtering options (on currently bound texture)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// load and generate the texture
		int width, height, nrChannels;
		unsigned char *data = stbi_load(path, &width, &height,
		&nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
			GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else { printf("Failed to load texture at [%s]\n", path); }
	}

	void DrawGameShape(float x, float y, const char* texturePath)
	{
		// Shape square vertices
		GLfloat vVertices[] = 	{
								 x,      y,      0.0f,	// Top left
								 x+0.2f, y,      0.0f,	// Top right
								 x+0.2f, y-0.2f, 0.0f,	// Bottom right
								 x,      y-0.2f, 0.0f	// Bottom left
								};
		// Load the vertex data
		glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
		glEnableVertexAttribArray ( 0 );

		// Texture coord
		GLfloat texCoord[] = 	{
								 0.0f, 0.0f,		// Bottom left
								 1.0f, 0.0f,		// Bottom right
								 1.0f, 1.0f,		// Top right
								 0.0f, 1.0f,		// Top left
								};
		// Load the texture data
		glVertexAttribPointer ( 2, 2, GL_FLOAT, GL_FALSE, 0, texCoord );
		glEnableVertexAttribArray ( 2 );

		LoadTexture(texturePath);

		glDrawArrays ( GL_QUADS, 0, 4 );
	}

	void DrawGameBoard()
	{
		for(int i = 0; i < 10; i++)
		{
			float stepY = (float)i/(float)5;
			for(int j = 0; j< 10; j++)
			{
				float stepX = (float)j/(float)5;

				std::string texturePathStr = GameLogic::GetShapeAt(i,j).GetTexturePath();
				const char* texturePath = texturePathStr.c_str();

				DrawGameShape(( -1.0f + stepX ), ( 1.0f - stepY ), texturePath);
			}
		}
	}
	
	char vShaderStr[] =
		"#version 300 es \n"
		"\n"
		"layout(location = 0) in vec4 a_position; \n"
		"layout(location = 2) in vec2 a_textures; \n"
		"out vec2 v_textures; \n"
		"\n"
		"void main() \n"
		"{ \n"
		" gl_Position = a_position; \n"
		" v_textures = a_textures; \n"
		"} \n";
	
	char fShaderStr[] =
		"#version 300 es \n"
		"precision mediump float; \n"
		"\n"
		"in vec2 v_textures; \n"
		"out vec4 fragColor; \n"
		"uniform sampler2D ourTexture; \n"
		"\n"
		"void main() \n"
		"{ \n"
		" fragColor =  texture(ourTexture, v_textures); \n"
		"} \n";

	GLFWRenderer::GLFWRenderer(std::shared_ptr<Context> context)
		: mContext(std::move(context))
	{
		// Prepare vertex shader
		const char * tmpShader = vShaderStr;
		GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
		glShaderSource(vertexShader, 1, &tmpShader, NULL);
		glCompileShader(vertexShader);
		
		// Prepare fragment shader
		tmpShader = fShaderStr;
		GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
		glShaderSource(fragmentShader, 1, &tmpShader, NULL);
		glCompileShader(fragmentShader);

		// Create the program object
		GLuint programObject = glCreateProgram();
		glAttachShader ( programObject, vertexShader );
		glAttachShader ( programObject, fragmentShader );

		// Link the program
		glLinkProgram ( programObject );

		// Use the program object
		glUseProgram ( programObject );

		// Set the viewport
		GLint windowWidth, windowHeight;
    	glfwGetWindowSize(window(), &windowWidth, &windowHeight);
		glViewport ( 0, 0, windowWidth, windowHeight );
	}

	void GLFWRenderer::render() {
		// GL code begin

		// Clear the color buffer
		glClear ( GL_COLOR_BUFFER_BIT );

		DrawGameBoard();

		// GL code end
		glfwSwapBuffers(window());
	}

	bool GLFWRenderer::poll() {
		if (glfwWindowShouldClose(window())) {
			return false;
		}
		return true;
	}
}