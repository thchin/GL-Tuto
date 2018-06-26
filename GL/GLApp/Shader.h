#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
	public:
		// Constructor generates the shader
		Shader(const GLchar *vertexPath, const GLchar *fragmentPath);
		~Shader();
		void	Use();
		GLuint	getProgram();
	private:
		GLuint m_program;
};

#endif