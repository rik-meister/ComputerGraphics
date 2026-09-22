#include "Context.h"			// put the matching header first

//
// Context.cpp
//

// compiler includes
#include <iostream>

// vcpkg includes
#include <GL/glew.h>			// glew (MUST COME BEFORE GLFW!!)
#include <GLFW/glfw3.h>			// glfw

// project includes
// ...

/* to be called by the driver when we did a bad. */
void APIENTRY ErrorCallback(GLenum Source, GLenum Type, GLuint Id, GLenum Severity,
	GLsizei Length, const GLchar* Message,
	const void* UserParam)
{
	std::cerr << Message << std::endl;
}

namespace aie
{
	bool Context::Init(int Width, int Height, const char* Title)
	{
		// init GLFW
		glfwInit();
		Window = glfwCreateWindow(Width, Height, Title, nullptr, nullptr);
		glfwMakeContextCurrent(Window); //<-- has to happen BEFORE we start initializing OpenGL, OpenGL needs a space to render into

		// init GLEW and OpenGL
		glewInit();

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		glDebugMessageCallback(ErrorCallback, 0);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, true);

		// print out some diagnostics
		std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

		glEnable(GL_BLEND);			// enable blending
		glEnable(GL_DEPTH_TEST);	// enable depth testing (so closer objects obscure farther objects)
		glEnable(GL_CULL_FACE);		// enable cull face (don't render backfaces)

		// configure how those things work
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthFunc(GL_EQUAL);
		glFrontFace(GL_CCW);	// front faces use CCW winding order
		glCullFace(GL_BACK);	// cull backfaces

		glClearColor(0.25f, 0.25f, 0.25f, 1.0f); //<--- (Red, Green, Blue, Alpha)

		return true;
	}
	void Context::Tick()
	{
		glfwSwapBuffers(Window);
		glfwPollEvents();
	}
	void Context::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void Context::Term()
	{
		glfwDestroyWindow(Window);
		Window = nullptr;

		glfwTerminate();
	}
	bool Context::ShouldClose() const
	{
		return glfwWindowShouldClose(Window);
	}
}