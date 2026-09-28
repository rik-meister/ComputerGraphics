/*
* 1. Create a window and set it as the rendering context (GLFW)
* 2. Start OpenGL-ing (GLEW)
* 3. Setting up OpenGL functions to actually do something
*
* GOAL: Put a (RED) triable on screen.
*/

#include <glm/glm.hpp>						// for mat4
#include <glm/ext/matrix_transform.hpp>		// for identity

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

	Vertex PlaneVerts[] =
	{
		{ { -.5f, -.5f, 0.0f, 1.0f } }, // bottom left
		{ {  .5f, -.5f, 0.0f, 1.0f } }, // bottom right
		{ { -.5f,  .5f, 0.0f, 1.0f } }, // bottom left
		{ {  .5f,  .5f, 0.0f, 1.0f } }	// top middle
	};
	GLuint PlaneIndices[] =
	{
			0, 1, 2, // bottom left triangle
			2, 1, 3  // top right triangle
	};
	//GeometryManaged BasicPlaneGeoManaged(PlaneVerts, PlaneIndices);
	Geometry BasicPlaneGeo = MakeGeometry(PlaneVerts, 4, PlaneIndices, 6);

	const char* BasicVert =
		"#version 430 core\n"
		"layout (location = 0) in vec4 position;\n"
		"void main() { gl_Position = position; }";

	const char* BasicFrag =
		"#version 430 core\n"
		"out vec4 outColor;\n"
		"void main() { outColor = vec4(1.0, 0.0, 0.0, 1.0); }";
	Shader BasicShad = MakeShader(BasicVert, BasicFrag);
	//Shader BasicShadFromFile = LoadShader("res/Basic.vert", "res/Basic.frag");

	glm::mat4 TriangleModel = glm::identity<glm::mat4>();
	TriangleModel = glm::translate(TriangleModel, glm::vec3(0, 1, 0)); // go up by 1 on Y-axis

	// V - View
	glm::mat4 Camera = glm::lookAt(glm::vec3(0, 1, 20), // eye (where is camera)
								   glm::vec3(0, 0, 0),	// center ( what are we looking at)
								   glm::vec3(0, 1, 0)); // up (orientation)

	// P -Projection

	while (!Window.ShouldClose())
	{
		Window.Tick();
		Window.Clear();
		Draw(BasicShad, BasicTriangleGeo);
	}

	Window.Term();

	return 0;
}