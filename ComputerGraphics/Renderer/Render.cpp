#include "Render.h"

#include <fstream>
#include <string>
#include <array>

#include <glm/gtc/type_ptr.hpp>	// vcpkg


// PUT THIS DEFINE IN A CPP
// ONLY IN ONE PLACE
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>			// vcpkg

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
		glEnableVertexAttribArray(0);	// position
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);	// color
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color)); // offset of 16 bytes

		glEnableVertexAttribArray(2);	// color
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV)); // offset of 32 bytes

		glEnableVertexAttribArray(3);	// normal
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)); // offset of 40 bytes

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
	Shader MakeShader(std::string_view VertSource, std::string_view FragSource)
	{
		return MakeShader(VertSource.data(), FragSource.data());
	}
	Shader LoadShader(std::string_view VertPath, std::string_view FragPath)
	{
		std::array<std::string, 2> Source;
		std::array<std::string_view, 2> Paths = { VertPath, FragPath };

		assert(Source.size() == Paths.size());

		for (size_t i = 0; i < Source.size(); ++i)
		{
			std::fstream FileStream(Paths[i].data());
			if (FileStream)
			{
				// @todo - Pre-allocate memory based on file size

				for (std::string Line; std::getline(FileStream, Line);)
				{
					Source[i] += Line + "\n";
				}
			}
		}
		return MakeShader(Source[0], Source[1]);
	}
	void FreeShader(Shader& Shad)
	{
		glDeleteProgram(Shad.Program);
		Shad = {};
	}

	Texture MakeTexture(unsigned Width, unsigned Height, unsigned Channels, const unsigned char* Pixels)
	{
		Texture NewTexture = { 0, Width, Height, Channels };
		
		// determine which channel enum to use for OpenGl
		GLenum OglFormat = GL_RED;
		switch (Channels)
		{
		case 1:
			OglFormat = GL_RED;
			break;
		case 2:
			OglFormat = GL_RG;
			break;
		case 3:
			OglFormat = GL_RGB;
			break;
		case 4:
			OglFormat = GL_RGBA;
			break;
		default:
			assert(false && "Invalid number of channels!!");
		}

		// generate texture
		glGenTextures(1, &NewTexture.Handle);


		// bind texture
		glBindTexture(GL_TEXTURE_2D, NewTexture.Handle);


		// buffer the texture
		glTexImage2D(GL_TEXTURE_2D, 0, OglFormat, Width, Height, 0, OglFormat, GL_UNSIGNED_BYTE, Pixels);


		// configure the texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		// unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);


		// return the object
		return NewTexture;
	}

	void FreeTexture(Texture& Tex)
	{
		glDeleteTextures(1, &Tex.Handle);
		Tex = {};
	}

	Texture LoadTexture(const char* TexPath)
	{
		unsigned char* RawPixelData = nullptr;

		// Configure stb_image (stbi)
		// by default, it loads images w/ the origin set to TOP LEFT but
		// OpenGL expects bottom left
		stbi_set_flip_vertically_on_load(true);

		// Load the pixel data
		int Width = 0;		// will be populated by stbi
		int Height = 0;		// will be populated by stbi
		int Format = 0;		// will be populated by stbi
		RawPixelData = stbi_load(TexPath, &Width, &Height, &Format, STBI_default);

		// Pass the pixel data to MakeTexture
		Texture NewTex = MakeTexture((unsigned)Width, (unsigned)Height, (unsigned)Format, RawPixelData);
		
		// Unload the pixel data from CPU (it will remain on the GPU)
		stbi_image_free(RawPixelData);

		// Return the texture
		return NewTex;
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
	void SetUniform(const Shader& Shad, GLuint Location, const glm::vec3& Value)
	{
		glProgramUniform3fv(Shad.Program, Location, 1, glm::value_ptr(Value));
	}
	void SetUniform(const Shader& Shad, GLuint Location, const glm::mat4& Value)
	{
		glProgramUniformMatrix4fv(Shad.Program, Location, 1, GL_FALSE, glm::value_ptr(Value));
	}
	void SetUniform(const Shader& Shad, GLuint Location, const Texture& Value, int TextureSlot)
	{
		// activate the texture slot to work with
		glActiveTexture(GL_TEXTURE0 + TextureSlot);

		// bind the texture to that slot
		glBindTexture(GL_TEXTURE_2D, Value.Handle);

		// assign that texture slot to the shader
		glProgramUniform1i(Shad.Program, Location, TextureSlot);
	}
}