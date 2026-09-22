/*
* 1. Create a window and set it as the rendering context (GLFW)
* 2. Start OpenGL-ing (GLEW)
* 3. Setting up OpenGL functions to actually do something
*
* GOAL: Put a (RED) triable on screen.
*/

#include "Context.h"
#include "Render.h"

//using aie::Context;
using namespace aie;	// at least it's not STD
int main()
{
	Context Window;
	Window.Init(640, 480, "Hello World");	// after this point, OpenGL is READY

	Vertex TriVerts[] =
	{
		{ { -.5f, -.5f, 0.0f, 1.0f } }, // bottom left
		{ {  .5f, -.5f, 0.0f, 1.0f } }, // bottom right
		{ { 0.0f,  .5f, 0.0f, 1.0f } }	// top middle
	};

	GLuint TriIndices[] = { 0, 1, 2 }; // bottom left, bottom right, top middle

	Geometry BasicTriangleGeo = MakeGeometry(TriVerts, 3, TriIndices, 3);

	while (!Window.ShouldClose())
	{
		Window.Tick();
		Window.Clear();
	}

	Window.Term();

	return 0;
}