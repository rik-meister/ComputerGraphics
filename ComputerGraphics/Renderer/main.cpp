/*
* 1. Create a window and set it as the rendering context (GLFW)
* 2. Start OpenGL-ing (GLEW)
* 3. Setting up OpenGL functions to actually do something
*
* GOAL: Put a (RED) triable on screen.
*/

#include <iostream>
#include <glm/glm.hpp>						// for glm::mat4
#include <glm/ext/matrix_transform.hpp>		// for glm::identity
#include <glm/ext/matrix_clip_space.hpp>	// for glm::perspective

#include "Context.h"
#include "Render.h"


//using aie::Context;
using namespace aie;	// at least it's not STD
int main()
{
	Context Window;
	Window.Init(640, 480, "Da World");	// after this point, OpenGL is READY

	Vertex TriVerts[] =
	{
		{ // bottom left
			{ -.5f, -.5f, 0.0f, 1.0f },	// position
			{ 1.0f, 0.0f, 0.0f, 1.0f },	// color
			{ 0.0f, 0.0f }				// UVs
		}, 
		{ // bottom right
			{  .5f, -.5f, 0.0f, 1.0f },	// position
			{ 0.0f, 1.0f, 0.0f, 1.0f },	// color
			{ 1.0f, 0.0f }				// UVs
		}, 
		{ // top middle
			{ 0.0f,  .5f, 0.0f, 1.0f },	// position
			{ 0.0f, 0.0f, 1.0f, 1.0f },	// color
			{ 0.5f, 1.0f }				// UVs
		}
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
	Shader BasicShadFromFile = LoadShader("res/Basic.vert", "res/Basic.frag");


	// M - Model
	glm::mat4 TriangleModel = glm::identity<glm::mat4>();
	TriangleModel = glm::translate(TriangleModel, glm::vec3(0, 1, 0)); // go up by 1 on Y-axis

	// V - View
	glm::mat4 Camera = glm::lookAt(glm::vec3(0, 1, 2), // eye (where is camera)
								   glm::vec3(0, 1, 0),	// center ( what are we looking at)
								   glm::vec3(0, 1, 0)); // up (orientation)

	// P -Projection
	glm::mat4 Projection = glm::perspective(glm::radians(45.f),	// V-FOV
											640.0f / 480.0f,	// ASPECT RATIO
											0.1f,				// NEAR
											100.0f);			// FAR

	Shader MVPShadFromFile = LoadShader("res/MVP.vert", "res/MVP.frag");
	SetUniform(MVPShadFromFile, 0, Projection);
	SetUniform(MVPShadFromFile, 1, Camera);
	SetUniform(MVPShadFromFile, 2, TriangleModel);

	Shader TexShadFromFile = LoadShader("res/Texture.vert", "res/Texture.frag");
	Texture TestTexture = LoadTexture("res/testtexture.png");
	SetUniform(TexShadFromFile, 0, Projection);
	SetUniform(TexShadFromFile, 1, Camera);
	SetUniform(TexShadFromFile, 2, TriangleModel);
	SetUniform(TexShadFromFile, 3, TestTexture, 0);

	Shader LightShadFromFile = LoadShader("res/Lighting.vert", "res/Lighting.frag");
	SetUniform(LightShadFromFile, 0, Projection);						// camera projection
	SetUniform(LightShadFromFile, 1, Camera);							// camera view matrix
	SetUniform(LightShadFromFile, 2, TriangleModel);					// model matrix
	SetUniform(LightShadFromFile, 3, TestTexture, 0);					// texture
	SetUniform(LightShadFromFile, 4, glm::vec3(0.2f, 0.2f, 0.2f));		// ambient light
	SetUniform(LightShadFromFile, 5, glm::vec3(0, 0, -1));				// light dir

	float Time = 0.0f;

	while (!Window.ShouldClose())
	{
		// fake time (always assumes 60fps)
		Time += 0.016f;

		TriangleModel = glm::identity<glm::mat4>();
		TriangleModel = glm::translate(TriangleModel, glm::vec3(0, 1, 0)); // go up by 1 on Y-axis
		TriangleModel = glm::rotate(TriangleModel, Time * 1.0f, glm::vec3(0, 1, 0)); // go up by 1 on Y-axis


		SetUniform(LightShadFromFile, 2, TriangleModel);

		Window.Tick();
		Window.Clear();

		Draw(LightShadFromFile, BasicTriangleGeo);
	}

	Window.Term();

	return 0;
}