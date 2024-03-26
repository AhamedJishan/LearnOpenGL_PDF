#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

int widthScreen = 800;
int heightScreen = 800;

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
	-0.5f,-0.5f,-0.5f, 0.0f, 0.0f,
	 0.5f,-0.5f,-0.5f, 1.0f, 0.0f,
	 0.5f, 0.5f,-0.5f, 1.0f, 1.0f,
	 0.5f, 0.5f,-0.5f, 1.0f, 1.0f,
	 -0.5f, 0.5f,-0.5f, 0.0f,1.0f,
	 -0.5f,-0.5f,-0.5f, 0.0f,0.0f,

	 -0.5f,-0.5f, 0.5f, 0.0f,0.0f,
	 0.5f,-0.5f, 0.5f, 1.0f,0.0f,
	 0.5f, 0.5f, 0.5f, 1.0f,1.0f,
	0.5f, 0.5f, 0.5f, 1.0f,1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f,1.0f,
	-0.5f,-0.5f, 0.5f, 0.0f,0.0f,

	-0.5f, 0.5f, 0.5f, 1.0f,0.0f,
	-0.5f, 0.5f,-0.5f, 1.0f,1.0f,
	-0.5f,-0.5f,-0.5f, 0.0f,1.0f,
	-0.5f,-0.5f,-0.5f, 0.0f,1.0f,
	-0.5f,-0.5f, 0.5f, 0.0f,0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f,0.0f,

	0.5f, 0.5f, 0.5f, 1.0f,0.0f,
	0.5f, 0.5f,-0.5f, 1.0f,1.0f,
	0.5f,-0.5f,-0.5f, 0.0f,1.0f,
	0.5f,-0.5f,-0.5f, 0.0f,1.0f,
	0.5f,-0.5f, 0.5f, 0.0f,0.0f,
	0.5f, 0.5f, 0.5f, 1.0f,0.0f,

	-0.5f,-0.5f,-0.5f, 0.0f,1.0f,
	0.5f,-0.5f,-0.5f, 1.0f,1.0f,
	0.5f,-0.5f, 0.5f, 1.0f,0.0f,
	0.5f,-0.5f, 0.5f, 1.0f,0.0f,
	-0.5f,-0.5f, 0.5f, 0.0f,0.0f,
	-0.5f,-0.5f,-0.5f, 0.0f,1.0f,

	-0.5f, 0.5f,-0.5f, 0.0f,1.0f,
	0.5f, 0.5f,-0.5f, 1.0f,1.0f,
	0.5f, 0.5f, 0.5f, 1.0f,0.0f,
	0.5f, 0.5f, 0.5f, 1.0f,0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f,0.0f,
	-0.5f, 0.5f,-0.5f, 0.0f,1.0f
};

glm::vec3 cubePositions[] = {
	 glm::vec3(0.0f, 0.0f, 0.0f),
	 glm::vec3(2.0f, 5.0f,-15.0f),
	 glm::vec3(-1.5f,-2.2f,-2.5f),
	 glm::vec3(-3.8f,-2.0f,-12.3f),
	 glm::vec3(2.4f,-0.4f,-3.5f),
	 glm::vec3(-1.7f, 3.0f,-7.5f),
	 glm::vec3(1.3f,-2.0f,-2.5f),
	 glm::vec3(1.5f, 2.0f,-2.5f),
	 glm::vec3(1.5f, 0.2f,-1.5f),
	 glm::vec3(-1.3f, 1.0f,-1.5f)
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

	GLFWwindow *window = glfwCreateWindow(widthScreen, heightScreen, "LearnOpenGL", NULL, NULL);
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

	glViewport(0, 0, widthScreen, heightScreen);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Vertex Array Object, Vertex Buffer Object, Element Buffer Object
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBindVertexArray(0);

	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);

	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);

	//Texture1
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, noChannels;
	unsigned char* data = stbi_load("src/res/textures/container.jpg", &width, &height, &noChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(texture1);
	}
	else
	{
		std::cout << "Failed to load the Texture!\n";
	}

	// Texture2
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("src/res/textures/awesomeface.png", &width, &height, &noChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(texture2);
	}
	else
	{
		std::cout << "Failed to load the Texture!\n";
	}
	stbi_image_free(data);

	Shader shader("src/res/shaders/vertex.vert", "src/res/shaders/fragment.frag");

	shader.Use();
	shader.SetInt("texture1", 0);
	shader.SetInt("texture2", 1);

	// Model, view, Projection matrices
	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 view = glm::mat4(1.0f);

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float)(widthScreen / heightScreen), 0.1f, 100.0f);

	// Render Loop
	while (!glfwWindowShouldClose(window))
	{
		ProcessInputs(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Wireframe
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		shader.Use();

		const float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		shader.SetMat4("view", view);
		shader.SetMat4("projection", projection);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			shader.SetMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Check call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}