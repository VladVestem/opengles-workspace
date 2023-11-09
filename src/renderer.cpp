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
	char vShaderStr[] =
		"#version 300 es \n"
		"layout(location = 0) in vec4 vPosition; \n"
		"void main() \n"
		"{ \n"
		" gl_Position = vPosition; \n"
		"} \n";
	
	char fShaderStr[] =
		"#version 300 es \n"
		"precision mediump float; \n"
		"out vec4 fragColor; \n"
		"void main() \n"
		"{ \n"
		" fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 ); \n"
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

		// Triangle vertices
		GLfloat vVertices[] = 	{
								 0.0f, 0.5f, 0.0f,
								-0.5f, -0.5f, 0.0f,
								 0.5f, -0.5f, 0.0f
								};

		// Load the vertex data
		glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
		glEnableVertexAttribArray ( 0 );

		glDrawArrays ( GL_TRIANGLES, 0, 3 );

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