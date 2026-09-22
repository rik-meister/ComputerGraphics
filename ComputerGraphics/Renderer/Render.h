#pragma once

#include <GL/glew.h>	// for GLuint (OpenGL-friendly types)
#include <glm/glm.hpp>	// for glm::vec4

namespace aie
{
	struct Vertex
	{
		glm::vec4 Pos;
	};

	struct Geometry
	{
		GLuint Vao = 0, Vbo = 0, Ibo = 0;	// buffer names
		GLuint Size = 0;			// index count
	};

	struct Shader
	{
		GLuint Program; // shader program name
	};

	Geometry MakeGeometry(const Vertex* const Verts, GLsizei VertCount,
						  const GLuint* const Indices, GLsizei IndexCount);
	void FreeGeometry(Geometry& Geo);

	Shader MakeShader(const char* VertSource, const char* FragSource);
	void FreeShader(Shader& Shad);

	void Draw(const Shader& Shad, const Geometry& Geo);
}