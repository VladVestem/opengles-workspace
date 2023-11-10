#include <renderer.hpp>
#include <exception.hpp>

#include <memory>
#include <optional>
#include <cassert>
#include <array>

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace opengles_workspace
{ 
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
	
	char vShaderStr[] =
		"#version 300 es \n"
		"\n"
		"layout(location = 0) in vec4 a_position; \n"
		"layout(location = 1) in vec4 a_colour; \n"
		"out vec4 v_colour; \n"
		"\n"
		"void main() \n"
		"{ \n"
		" gl_Position = a_position; \n"
		" v_colour = a_colour; \n"
		"} \n";
	
	char fShaderStr[] =
		"#version 300 es \n"
		"precision mediump float; \n"
		"\n"
		"in vec4 v_colour; \n"
		"out vec4 fragColor; \n"
		"\n"
		"void main() \n"
		"{ \n"
		" fragColor = v_colour; \n"
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
		DrawCheckerboard(20);

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