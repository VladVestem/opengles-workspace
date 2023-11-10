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

namespace opengles_workspace
{ 
	void LoadTexture(std::string path)
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
		unsigned char *data = stbi_load(path.c_str(), &width, &height,
		&nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
			GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else { printf("Failed to load texture\n"); }
	}

	void DrawTriangle(float x, float y, float z, float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f)
	{
		// Triangle vertices
		GLfloat vVertices[] = 	{
								 x, y, z,			// Top
								 x-0.5f, y-1.0f, z,	// Bottom left
								 x+0.5f, y-1.0f, z	// Bottom right
								};
		// Load the vertex data
		glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
		glEnableVertexAttribArray ( 0 );

		// Colour array
		float colours[] =	{
							 red, green, blue, alpha,
							 red, green, blue, alpha,
							 red, green, blue, alpha
							};
		// Load the colour data
		glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, 0, colours );
		glEnableVertexAttribArray ( 1 );

		glDrawArrays ( GL_TRIANGLES, 0, 3 );
	}

	void DrawSquare(float x, float y, float z, float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f)
	{
		// Triangle vertices
		GLfloat vVertices[] = 	{
								 x, y, z,			// Top left
								 x+0.2f, y, z,		// Top right
								 x+0.2f, y-0.2f, z,	// Bottom right
								 x, y-0.2f, z		// Bottom left
								};
		// Load the vertex data
		glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
		glEnableVertexAttribArray ( 0 );

		// Colour array
		float colours[] =	{
							 red, green, blue, alpha,
							 red, green, blue, alpha,
							 red, green, blue, alpha,
							 red, green, blue, alpha
							};
		// Load the colour data
		glVertexAttribPointer ( 1, 4, GL_FLOAT, GL_FALSE, 0, colours );
		glEnableVertexAttribArray ( 1 );

		glDrawArrays ( GL_QUADS, 0, 4 );
	}

	void DrawSquareTextured(float x, float y, float z, float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f)
	{
		// Triangle vertices
		GLfloat vVertices[] = 	{
								 x, y, z,			// Top left
								 x+0.2f, y, z,		// Top right
								 x+0.2f, y-0.2f, z,	// Bottom right
								 x, y-0.2f, z		// Bottom left
								};
		// Load the vertex data
		glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
		glEnableVertexAttribArray ( 0 );

		// Colour array
		float colours[] =	{
							 red, green, blue, alpha,
							 red, green, blue, alpha,
							 red, green, blue, alpha,
							 red, green, blue, alpha
							};
		// Load the colour data
		glVertexAttribPointer ( 1, 4, GL_FLOAT, GL_FALSE, 0, colours );
		glEnableVertexAttribArray ( 1 );

		// Texture coord
		GLfloat texCoord[] = 	{
								 1.0f, 1.0f,		// Top right
								 0.0f, 1.0f,		// Top left
								 0.0f, 0.0f,		// Bottom left
								 1.0f, 0.0f,		// Bottom right
								};
		// Load the vertex data
		glVertexAttribPointer ( 2, 2, GL_FLOAT, GL_FALSE, 0, texCoord );
		glEnableVertexAttribArray ( 2 );

		LoadTexture("../checkerboard.jpg");

		glDrawArrays ( GL_QUADS, 0, 4 );
	}

	void DrawCheckerboard(int size)
	{
		for(int i = 0; i < size; i++)
		{
			float stepX = (float)i/(size/2);
			float isRed = (i % 2 != 0) ? 1.0f : 0.0f;
			for(int j = 0; j< size; j++)
			{
				float stepY = (float)j/(size/2);
				isRed = (isRed == 1.0f) ? 0.0f : 1.0f; // Invert colour on every new line
				DrawSquare((-1.0f+stepX), (1.0f-stepY), 0.0f, 1.0f, (1.0f-isRed), (1.0f-isRed));
			}
		}
	}

	void DrawCheckerboardTextured()
	{
		for(int i = 0; i < 5; i++)
		{
			float stepX = (float)i/5;
			for(int j = 0; j< 5; j++)
			{
				float stepY = (float)j/5;
				DrawSquareTextured((-0.5f+stepX), (0.5f-stepY), 0.0f);
			}
		}
	}
	
	char vShaderStr[] =
		"#version 300 es \n"
		"\n"
		"layout(location = 0) in vec4 a_position; \n"
		"layout(location = 1) in vec4 a_colour; \n"
		"layout(location = 2) in vec2 a_textures; \n"
		"out vec4 v_colour; \n"
		"out vec2 v_textures; \n"
		"\n"
		"void main() \n"
		"{ \n"
		" gl_Position = a_position; \n"
		" v_colour = a_colour; \n"
		" v_textures = a_textures; \n"
		"} \n";
	
	char fShaderStr[] =
		"#version 300 es \n"
		"precision mediump float; \n"
		"\n"
		"in vec4 v_colour; \n"
		"in vec2 v_textures; \n"
		"out vec4 fragColor; \n"
		"uniform sampler2D ourTexture; \n"
		"\n"
		"void main() \n"
		"{ \n"
		" fragColor =  texture(ourTexture, v_textures); \n"
		"} \n";

	GLuint programObject;

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
		programObject = glCreateProgram();
		glAttachShader ( programObject, vertexShader );
		glAttachShader ( programObject, fragmentShader );

		// Link the program
		glLinkProgram ( programObject );

		// Use the program object
		glUseProgram ( programObject );
	}

	void GLFWRenderer::render() {
		// GL code begin

		// Set the viewport
		GLint windowWidth, windowHeight;
    	glfwGetWindowSize(window(), &windowWidth, &windowHeight);
		glViewport ( 0, 0, windowWidth, windowHeight );

		// Clear the color buffer
		glClear ( GL_COLOR_BUFFER_BIT );

		//DrawTriangle(0.0f, 0.5f, 0.0f);
		//DrawSquare(-1.0f, 1.0f, 0.0f);
		//DrawCheckerboard(10);
		//DrawSquareTextured(-1.0f, 1.0f, 0.0f);
		DrawCheckerboardTextured();

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