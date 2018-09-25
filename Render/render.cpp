#include "renderr.h"
#include "GLEW/glew.h"
#include "GLM/gtc/type_ptr.hpp"

geometry makeGeometry(vertex * verts, size_t vertCount, unsigned * indices, size_t indexCount)
{
	// allocate buffers (VAO, VBO, and IBO)
	GLuint vao, vbo, ibo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	//bind
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	//populate buffers
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertCount, verts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indexCount, indices, GL_STATIC_DRAW);

	//describe the data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)16);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)32);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)48);

	//unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return { vao, vbo, ibo, indexCount };
}

void freeGeometry(geometry &geo)
{
	glDeleteVertexArrays(1, &geo.vao);
	glDeleteBuffers(1, &geo.vbo);
	glDeleteBuffers(1, &geo.ibo);
}

shader makeShader(const char * vertSrc, const char * fragSrc)
{
	GLuint vert = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vert, 1, &vertSrc, 0);
	glShaderSource(frag, 1, &fragSrc, 0);

	glCompileShader(vert);
	glCompileShader(frag);

	GLuint prog = glCreateProgram();

	glAttachShader(prog, vert);
	glAttachShader(prog, frag);

	glLinkProgram(prog);

	glDeleteShader(vert);
	glDeleteShader(frag);

	return { prog };
}

void freeShader(shader & shad)
{
	glDeleteProgram(shad.program);
}

texture makeTexture(unsigned width, unsigned height, unsigned channels, const unsigned char * pixels)
{
	GLenum oglFormat = 0;
	switch (channels)
	{
	case 1: oglFormat = GL_RED; break;
	case 2: oglFormat = GL_RG; break;
	case 3: oglFormat = GL_RGB; break;
	case 4: oglFormat = GL_RGBA; break;
	}

	texture newTex = { 0,width,height,channels };

	glGenTextures(1, &newTex.handle);
	glBindTexture(GL_TEXTURE_2D, newTex.handle);

	glTexImage2D(GL_TEXTURE_2D, 0, oglFormat, width, height, 0, oglFormat, GL_UNSIGNED_BYTE, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	return newTex;
}

void freeTexture(texture & tex)
{
	glDeleteTextures(1, &tex.handle);
	tex = {};
}

void draw(const shader & shad, const geometry & geo)
{
	glUseProgram(shad.program);
	glBindVertexArray(geo.vao);
	glDrawElements(GL_TRIANGLES, geo.size, GL_UNSIGNED_INT, 0);
}

void setUniform(const shader & shad, GLuint location, const glm::mat4 & value)
{
	glProgramUniformMatrix4fv(shad.program, location, 1, GL_FALSE, glm::value_ptr(value));
}

void setUniform(const shader & shad, GLuint location, const texture & value, GLuint textureSlot)
{
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, value.handle);
	glProgramUniform1i(shad.program, location, textureSlot);
}

void setUniform(const shader & shad, GLuint location, const glm::vec3 & value)
{
	glProgramUniform3fv(shad.program, location, 1, glm::value_ptr(value));
}