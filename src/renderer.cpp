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

#include <ft2build.h>
#include FT_FREETYPE_H

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

	FT_Face face;
	void InitFT()
	{
		// FreeType
		// ------------------------------------------------------------------------------
		// Init FreeType
		FT_Library ft;
		if(FT_Init_FreeType(&ft))
		{
			fprintf(stderr, "Could not init freetype library\n");
		}
		// Load font as FT_Face
		if(FT_New_Face(ft, "../images/font.ttf", 0, &face))
		{
			fprintf(stderr, "Could not open font\n");
		}
		// Set font size
		FT_Set_Pixel_Sizes(face, 0, 48);
		// ------------------------------------------------------------------------------
	}

	void DrawGameText(float x, float y, FT_Face& face, char charToRender)
	{
		// Load glyph of character
		if(FT_Load_Char(face, charToRender, FT_LOAD_RENDER))
		{
			fprintf(stderr, "Could not load character '%d'\n", charToRender);
		}

		float bitmapW = (float)face->glyph->bitmap.width/(float)200;
		float bitmapH = (float)face->glyph->bitmap.rows/(float)200;

		// Score square vertices
		GLfloat vVertices[] = 	{
								 x,			y,			0.0f,	// Top left
								 x+bitmapW,	y,			0.0f,	// Top right
								 x+bitmapW,	y-bitmapH,	0.0f,	// Bottom right
								 x,			y-bitmapH,	0.0f	// Bottom left
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

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// set the texture wrapping/filtering options (on currently bound texture)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// load and generate the texture
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		glGenerateMipmap(GL_TEXTURE_2D);

		glDrawArrays ( GL_QUADS, 0, 4 );
	}

	void DrawGameScore()
	{
		int score = GameLogic::GetScore();
		std::string scoreString = std::to_string(score) + " score";

		float stepX = 0.0f;
		for(char c : scoreString)
		{
			DrawGameText(-1.0f + stepX, 1.0f, face, c);
			stepX += (float)face->glyph->bitmap.width/(float)100;
		}
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

				DrawGameShape(( -1.0f + stepX ), ( 0.8f - stepY ), texturePath);
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

		InitFT();
	}

	void GLFWRenderer::render() {
		// GL code begin

		// Clear the color buffer
		glClear ( GL_COLOR_BUFFER_BIT );

		DrawGameBoard();
		DrawGameScore();

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