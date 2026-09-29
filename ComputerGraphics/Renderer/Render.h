#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <span>

#include <GL/glew.h>	// for GLuint (OpenGL-friendly types)
#include <glm/glm.hpp>	// for glm::vec4

namespace aie
{
	struct Vertex	// Changes to this will require changing MakeGeometry
	{
		glm::vec4 Pos = {};		// 0 - Position
		glm::vec4 Color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);	// 1 - Color
		glm::vec2 UV = {};
	};

	struct Geometry
	{
		GLuint Vao = 0, Vbo = 0, Ibo = 0;	// buffer names
		GLuint Size = 0;			// index count
	};

	struct Texture
	{
		GLuint Handle = 0; // texture name
		unsigned int Width = 0, Height = 0, Channels = 0;
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
	// Returns a shader whose source code is provided via string_view which can be constructed by an old school C-String or a modern std:: string object

	Shader MakeShader(std::string_view VertSource, std::string_view FragSource);
	Shader LoadShader(std::string_view VertPath, std::string_view FragPath);
	void FreeShader(Shader& Shad);

	Texture MakeTexture(unsigned Width, unsigned Height, unsigned Channels, const unsigned char* Pixels);
	void FreeTexture(Texture& Tex);
	Texture LoadTexture(const char* TexPath);

	void Draw(const Shader& Shad, const Geometry& Geo);
	void SetUniform(const Shader& Shad, GLuint Location, const glm::mat4& Value);
	void SetUniform(const Shader& Shad, GLuint Location, const Texture& Value, int TextureSlot);
}