#include "Render.h"

#include <fstream>
#include <array>

#include <glm/gtc/type_ptr.hpp>

namespace aie
{
	Geometry MakeGeometry(const Vertex* const Verts, GLsizei VertCount, const GLuint* const Indices, GLsizei IndexCount)
	{
		// create a return value object
		Geometry NewGeo = {};
		NewGeo.Size = IndexCount;

		// generate buffers
		glGenVertexArrays(1, &NewGeo.Vao);  // make 1 vertex array object (VAO)
		glGenBuffers(1, &NewGeo.Vbo);		// make 1 vertex buffer object (VBO)
		glGenBuffers(1, &NewGeo.Ibo);		// make 1 index buffer object (IBO)

		// bind buffers
		glBindVertexArray(NewGeo.Vao);
		glBindBuffer(GL_ARRAY_BUFFER, NewGeo.Vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NewGeo.Ibo);

		// buffer buffers
		glBufferData(GL_ARRAY_BUFFER, VertCount * sizeof(Vertex), Verts, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexCount * sizeof(GLuint), Indices, GL_STATIC_DRAW);

		// describe vertex data
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		// unbind buffers
		glBindVertexArray(0);						// unbind VAO first !!
		glBindBuffer(GL_ARRAY_BUFFER, 0);			// unbind VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// unbind IBO

		//return the object
		return NewGeo;
	}

	void FreeGeometry(Geometry& Geo)
	{
		glDeleteBuffers(1, &Geo.Ibo);
		glDeleteBuffers(1, &Geo.Vbo);
		glDeleteVertexArrays(1, &Geo.Vao);

		// clear the geo object
		Geo = {};
	}
	Shader MakeShader(const char* VertSource, const char* FragSource)
	{
		// make the shader object
		Shader NewShad = {};
		NewShad.Program = glCreateProgram();

		// create shaders
		GLuint Vert = glCreateShader(GL_VERTEX_SHADER);
		GLuint Frag = glCreateShader(GL_FRAGMENT_SHADER);

		// compile shaders
		glShaderSource(Vert, 1, &VertSource, 0);
		glShaderSource(Frag, 1, &FragSource, 0);
		glCompileShader(Vert);
		glCompileShader(Frag);
		// TODO: add error checking code to ensure that the shader is well-formed!! (aka actually compiles)

		// attach shaders to shader program
		glAttachShader(NewShad.Program, Vert);
		glAttachShader(NewShad.Program, Frag);

		// link shader program
		glLinkProgram(NewShad.Program);
		// TODO: add error checking code to ensure that the shader actually links!!

		// delete shader
		glDeleteShader(Vert);
		glDeleteShader(Frag);

		// return the shader object
		return NewShad;
	}
	//Shader MakeShader(std::string_view VertSource, std::string_view FragSource)
	//{
	//	return MakeShader(VertSource.data(), FragSource.data());
	//}
	//Shader LoadShader(std::string_view VertPath, std::string_view FragPath)
	//{
	//	std::array<std::string, 2> Source;
	//	std::array<std::string_view, 2> Paths = { VertPath, FragPath };

	//	assert(Source.size() == Paths.size());

	//	for (size_t i = 0; i < Source.size(); ++i)
	//	{
	//		std::fstream FileStream(Paths[i].data());
	//		if (FileStream)
	//		{
	//			// @todo - Pre-allocate memory based on file size

	//			for (std::strong_ordering Line; std::getline(FileStream, Line);)
	//			{
	//				Source[i] += Line + "\n";
	//			}
	//		}
	//	}
	//	return MakeShader(Source[0], Source[1]);
	//}
	void FreeShader(Shader& Shad)
	{
		glDeleteProgram(Shad.Program);
		Shad = {};
	}

	void Draw(const Shader& Shad, const Geometry& Geo)
	{
		// bind the shader program
		glUseProgram(Shad.Program);
		
		// bind VAO
		glBindVertexArray(Geo.Vao);

		// draw!!
		glDrawElements(GL_TRIANGLES, Geo.Size, GL_UNSIGNED_INT, nullptr);
	}
	void SetUniform(const Shader& Shad, GLuint Location, const glm::mat4& Value)
	{
		glProgramUniformMatrix4fv(Shad.Program, Location, 1, GL_FALSE, glm::value_ptr(Value));
	}
}