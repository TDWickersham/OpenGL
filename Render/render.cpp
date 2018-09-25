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