#pragma once

#include <iostream>
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

	/*
	*Wrapped version of Geometry that follows Resource Acquisition is Initialization (RAII)
	* 
	*See also: https://en.cppreference.com/cpp/language/raii
	*/
	//class GeometryManaged
	//{
	//	Geometry Geo;
	//public:
	//	GeometryManaged();	// empty/not-geometry
	//	GeometryManaged(const GeometryManaged& Other) = delete; // not implemented (DO NOT COPY CONSTRUCT)
	//	GeometryManaged& operator=(const GeometryManaged& Other) = delete;
	//	~GeometryManaged();

	//	GeometryManaged(const std::span<const Vertex>& Verts, const std::span < const GLuint& Indices);

	//	operator Geometry() const { return Geo; }
	//	operator Geometry& () { return Geo; }
	//};
	struct Shader
	{
		GLuint Program; // shader program name
	};

	Geometry MakeGeometry(const Vertex* const Verts, GLsizei VertCount,
						  const GLuint* const Indices, GLsizei IndexCount);
	//Geometry MakeGeometry(const std::vector<Vertex>& Verts, const std::vector<GLuint>& Indices);

	void FreeGeometry(Geometry& Geo);

	Shader MakeShader(const char* VertSource, const char* FragSource);
	//Shader MakeShader(std::string_view VertSource, std::string_view FragSource);
	//Shader LoadShader(std::string_view VertPath, std::string_view FragPath);
	void FreeShader(Shader& Shad);

	void Draw(const Shader& Shad, const Geometry& Geo);
	void SetUniform(const Shader& Shad, GLuint Location, const glm::mat4& Value);
}