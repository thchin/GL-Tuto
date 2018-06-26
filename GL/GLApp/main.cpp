#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <gtx/string_cast.hpp>

#include "GameEngine.h"
#include "Shader.h"

//void	KeyCallBack(GLFWwindow *window, int key, int scancode, int action, int mode);
//void	ScrollCallBack(GLFWwindow *window, double xOffset, double yOffset);
//void	MouseCallBack(GLFWwindow *window, double xPos, double yPos);
//void	DoMovement();

void		DoMovement(GLfloat deltaTime);
void		KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void		ScrollCallback(GLFWwindow *window, double xOffset, double yOffset);
void		MouseCallback(GLFWwindow *window, double xPos, double yPos);

const GLint WIDTH = 800, HEIGHT = 600;

bool		keys[1024];
bool		firstMouse = true;

GLfloat		lastX = WIDTH / 2.0f;
GLfloat		lastY = WIDTH / 2.0F;
GLfloat		lastFrame = 0.0f;

Camera		camera(glm::vec3(0.0f, 0.0f, 10.0f));


int main()
{
	GameEngine	gameEngine;

	// Init GLEW GLFW create a window and set viewport
	gameEngine.InitSystems();

	// Set Input Callback
	glfwSetKeyCallback(gameEngine.GetWindow(), KeyCallback);
	glfwSetCursorPosCallback(gameEngine.GetWindow(), MouseCallback);
	glfwSetScrollCallback(gameEngine.GetWindow(), ScrollCallback);

	glfwSetInputMode(gameEngine.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	Shader		ourShader("core.vert", "core.frag");
	
	// Set up vertex data (and buffer(s)) and configure vertex attributes
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	//gameEngine.CopyData(vertices);

	GLuint	VAO, VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Copy the vertices array in a buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// Set the texture coordinate attribute pointers
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Unbind VAO
	glBindVertexArray(0);

	// Texture Loading

	GLuint	texture;
	int	width, height;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Set texture axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load image in Opengl
	unsigned char	*image = SOIL_load_image("Image/wall.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	if (image != nullptr)
	{
		// Generate texture in opengl
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		// Generate Mipmap
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "ERROR::MAIN.CPP::LOAD_IMAGE_FAILED" << std::endl;

	// Free image variable (now in opengl)
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);



	// Game loop
	while (!glfwWindowShouldClose(gameEngine.GetWindow()))
	{
		GLfloat	currentFrame = (GLfloat)glfwGetTime();
		GLfloat deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check for events/input
		glfwPollEvents();
		DoMovement(deltaTime);

		// Render
		// Clear the color buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
/*		// Set transformation matrix
		glm::mat4 transform(1); // Set to identity matrix
		transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
		transform = glm::rotate(transform, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(0.0f, 0.0f, 1.0f));

		GLint	transformLocation = glGetUniformLocation(ourShader.getProgram(), "transform");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));
*/
		// Select an active texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		// Set uniform Location

		glUniform1i(glGetUniformLocation(ourShader.getProgram(), "ourTexture"), 0);
		
		ourShader.Use();

		GLint	modelLoc = glGetUniformLocation(ourShader.getProgram(), "model");
		GLint	viewLoc = glGetUniformLocation(ourShader.getProgram(), "view");
		GLint	projLoc = glGetUniformLocation(ourShader.getProgram(), "projection");

		glm::mat4	projection(1);
		projection = glm::perspective(camera.GetZoom(), 4.0f / 3.0f, 0.1f, 1000.0f);

		glm::mat4	view(1);
		view = camera.GetViewMatrix();
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));


		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4	model(1);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(gameEngine.GetWindow());
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

	return (EXIT_SUCCESS);
}

void	KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024)
	{
		if (GLFW_PRESS == action)
			keys[key] = true;
		else if (GLFW_RELEASE == action)
			keys[key] = false;
	}
}

void	ScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll((GLfloat)yOffset);
}

void	MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = (GLfloat)xPos;
		lastY = (GLfloat)yPos;
		firstMouse = false;
	}
	GLfloat	xOffset = (GLfloat)xPos - lastX;
	GLfloat	yOffset = lastY - (GLfloat)yPos;
	lastX = (GLfloat)xPos;
	lastY = (GLfloat)yPos;

	camera.ProcessMouse(xOffset, yOffset);
}

void	DoMovement(GLfloat deltaTime)
{
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}