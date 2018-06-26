#include "GameEngine.h"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

GameEngine::GameEngine()
{
	this->window = nullptr;
	this->screenWidth = 0;
	this->screenHeight = 0;
}

GameEngine::~GameEngine()
{
}

void	GameEngine::InitSystems()
{
	// Init GLFW
	glfwInit();

	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	this->window = glfwCreateWindow(800, 600, "gameapp", nullptr, nullptr);

	glfwGetFramebufferSize(this->window, &this->screenWidth, &this->screenHeight);

	if (nullptr == this->window)
	{
		std::cout << "ERROR::GAMEENGINE.CPP::CREATE_WINDOW_FAILED" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(this->window);



	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "ERROR::GAMEENGINE.CPP::GLEW_INIT_FAILED" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	// Define the viewport dimensions
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void		GameEngine::CopyData(GLfloat vertices[])
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	// Bind VAO
	glBindVertexArray(this->VAO);

	// Copy the vertices array in a buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// Set the texture coordinate attribute pointers
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Unbind VAO
	glBindVertexArray(0);
}

GLFWwindow	*GameEngine::GetWindow()
{
	return (this->window);
}

GLuint		&GameEngine::GetVAO()
{
	return (this->VAO);
}

GLuint		&GameEngine::GetVBO()
{
	return (this->VBO);
}