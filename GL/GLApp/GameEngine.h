#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.h"

extern Camera		camera;

class GameEngine
{
	public:
		GameEngine();
		~GameEngine();
		void				InitSystems();
		void				CopyData(GLfloat vertices[]);
		GLFWwindow			*GetWindow();
		GLuint		&GetVBO();
		GLuint		&GetVAO();

	private:
		GLFWwindow	*window;
		int			screenWidth, screenHeight;
		GLuint		VBO, VAO;
};

