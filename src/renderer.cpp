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

#include <ft2build.h>
#include FT_FREETYPE_H

namespace opengles_workspace
{
	float scoreX = -0.9f;
	float scoreY = 1.0f;
	float boardX = -0.9f;
	float boardY = 0.8f;
	float boardSquareSize = 0.2f;

	float ItoYcoord(int I)
	{
		return boardY-(boardSquareSize*I);
	}

	float JtoXcoord(int J)
	{
		return boardX+(boardSquareSize*J);
	}

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
		else
		{
			printf("Failed to load texture at [%s]\n", path);
		}
	}

	FT_Face face;
	void InitFT()
	{
		// Init FreeType
		FT_Library ft;
		if(FT_Init_FreeType(&ft))
		{
			fprintf(stderr, "Could not init freetype library\n");
		}
		// Load font as FT_Face
		if(FT_New_Face(ft, "../font/font.ttf", 0, &face))
		{
			fprintf(stderr, "Could not open font\n");
		}
		// Set font size
		FT_Set_Pixel_Sizes(face, 0, 48);
	}

	void DrawGameText(float x, float y, FT_Bitmap bitmap)
	{
		// Get bitmap data and dimensions
		unsigned char* bitmapCharData = bitmap.buffer;
		float bitmapWidth = (float)bitmap.width/200.0f;
		float bitmapHeight = (float)bitmap.rows/200.0f;

		// Set center offsets to correctly draw the character in the center of designated coordinates
		float centerOffsetX = (boardSquareSize - bitmapWidth)/2.0f;
		float leftX = x + centerOffsetX;
		float rightX = leftX + bitmapWidth;

		float centerOffsetY = (boardSquareSize - bitmapHeight)/2.0f;
		float topY = y - centerOffsetY;
		float bottomY = topY - bitmapHeight;

		// Character square vertices
		GLfloat vVertices[] = 	{
								 leftX,		topY,		0.0f,	// Top left
								 rightX,	topY,		0.0f,	// Top right
								 rightX,	bottomY,	0.0f,	// Bottom right
								 leftX,		bottomY,	0.0f	// Bottom left
								};
		// Load the vertex data
		glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
		glEnableVertexAttribArray ( 0 );

		// Texture coordinates
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

		// Set the texture wrapping/filtering options (on currently bound texture)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Load and generate the texture
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmap.width, bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.buffer);
		glGenerateMipmap(GL_TEXTURE_2D);

		glDrawArrays ( GL_QUADS, 0, 4 );
	}

	void DrawGameScore(float x, float y)
	{
		int score = GameLogic::GetScore();
		std::string scoreString = "SCORE-" + std::to_string(score);

		float X = x;
		float Y = y;
		for(char charToRender : scoreString)
		{
			// Load glyph of character
			if(FT_Load_Char(face, charToRender, FT_LOAD_RENDER))
			{
				fprintf(stderr, "Could not load character '%d'\n", charToRender);
			}
			FT_Bitmap bitmap = face->glyph->bitmap;			

			DrawGameText(X, Y, bitmap);
			X += boardSquareSize;
		}
	}

	void DrawGameShape(float x, float y, const char* texturePath)
	{
		// Set correct coordinates
		float leftX = x;
		float rightX = leftX + boardSquareSize;

		float topY = y;
		float bottomY = topY - boardSquareSize;

		// Shape square vertices
		GLfloat vVertices[] = 	{
								 leftX,		topY,		0.0f,	// Top left
								 rightX,	topY,		0.0f,	// Top right
								 rightX,	bottomY,	0.0f,	// Bottom right
								 leftX,		bottomY,	0.0f	// Bottom left
								};
		// Load the vertex data
		glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
		glEnableVertexAttribArray ( 0 );

		// Texture coordinates
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

	void DrawGameBoard(float x, float y)
	{
		float X = x;
		float Y = y;

		for(int rows = 0; rows < GameLogic::gameBoardSize; rows++)
		{
			// Reset X coordinate every row
			X = x;
			for(int columns = 0; columns < GameLogic::gameBoardSize; columns++)
			{
				// Get texture path of shape at specific row & column
				std::string texturePathStr = GameLogic::GetShapeAt(rows,columns).GetTexturePath();
				const char* texturePath = texturePathStr.c_str();

				DrawGameShape(X, Y, texturePath);
				// Increase X coordinate after every collumn finished rendering
				X += boardSquareSize;
			}
			// Decrease Y coordinate after row finished rendering
			Y -= boardSquareSize;
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

		// Init FreeType
		InitFT();
	}

	void GLFWRenderer::render() {
		// GL code begin

		// Clear the color buffer
		glClear ( GL_COLOR_BUFFER_BIT );

		DrawGameBoard(boardX, boardY);
		DrawGameScore(scoreX, scoreY);

		// GL code end
		glfwSwapBuffers(window());
	}

	void GLFWRenderer::renderOnlyCursor(Direction direction)
	{
		// Get indexes of current shape
		int i = GameLogic::GetCurrentI();
		int j = GameLogic::GetCurrentJ();

		// Convert indexes to coordinates
		float x = JtoXcoord(j);
		float y = ItoYcoord(i);

		// Get texture path of current shape
		std::string texturePathStr = GameLogic::GetShapeAt(i,j).GetTexturePath();
		const char* texturePath = texturePathStr.c_str();

		// Redraw only current shape
		DrawGameShape(x, y, texturePath);

		switch (direction)
		{
		case UP:
			i++;
			break;
		case LEFT:
			j++;
			break;
		case DOWN:
			i--;
			break;
		case RIGHT:
			j--;
			break;
		default:
			break;
		}

		// Convert indexes to coordinates
		x = JtoXcoord(j);
		y = ItoYcoord(i);

		// Get texture path of current shape
		texturePathStr = GameLogic::GetShapeAt(i,j).GetTexturePath();
		texturePath = texturePathStr.c_str();

		// Redraw only current shape
		DrawGameShape(x, y, texturePath);

		glfwSwapBuffers(window());
	}

	bool GLFWRenderer::poll() {
		if (glfwWindowShouldClose(window())) {
			return false;
		}
		return true;
	}
}