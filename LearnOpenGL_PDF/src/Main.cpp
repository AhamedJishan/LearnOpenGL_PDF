#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "vendor/stb_image.h"

#include "Shader.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ProcessInputs(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

const float vertices[] = {
	  // Coordinates        // Colors			// Tex-Coord
	-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,	//0 Bottom-left
	 0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	1.0f, 0.0f,	//1 Bottom-right
	 0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,	//2 Top-right
	-0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f	//3 Top-left
};

unsigned int indices[] = {
	0, 1, 2,
	2, 3, 0
};


int main()
{
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 800, "LearnOpenGL", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create window!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!\n" << glfwGetVersionString();
		return -1;
	}

	glViewport(0, 0, 800, 800);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Vertex Array Object, Vertex Buffer Object, Element Buffer Object
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBindVertexArray(0);

	Shader shader("src/res/shaders/vertex.vert", "src/res/shaders/fragment.frag");

	// Render Loop
	while (!glfwWindowShouldClose(window))
	{
		ProcessInputs(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Wireframe
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		shader.Use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Check call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}