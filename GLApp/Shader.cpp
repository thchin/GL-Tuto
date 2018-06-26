#include "Shader.h"

Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath)
{
	// Retrieve the vertex/fragment source from filepath
	std::string		vertexCode;
	std::string		fragmentCode;
	std::ifstream	vShaderFile;
	std::ifstream	fShaderFile;
	// Ensures ifstream objects can throw exceptions
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	// Get Source Code
	try
	{
		// Open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// Close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	// Compile Shader
	GLuint vertex, fragment;
	GLint success;
	GLchar	infoLog[512];
	// Create the Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	// Replace the source code in the vertex Shader
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	// Compile the Vertex Shader
	glCompileShader(vertex);


	// Check if compilation succeeded
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		// Get the compilation log
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Create the Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	// Replace the source code in the Gragment Shader
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	// Compile the Gragment Shader
	glCompileShader(fragment);

	// Check if compilation succeeded
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		// Get the compilation log
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Create Shader Program
	m_program = glCreateProgram();
	// Attach Vertex Shader to Program
	glAttachShader(m_program, vertex);
	// Attach Fragment Shader to Program
	glAttachShader(m_program, fragment);
	// Link the program object
	glLinkProgram(m_program);

	// Check if linking succeeded
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		// Get the linking log
		glGetProgramInfoLog(m_program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << std::endl << infoLog << std::endl;
	}

	// Delete Shaders
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}


Shader::~Shader()
{
}

void	Shader::Use()
{
	glUseProgram(m_program);
}

GLuint Shader::getProgram()
{
	return (m_program);
}