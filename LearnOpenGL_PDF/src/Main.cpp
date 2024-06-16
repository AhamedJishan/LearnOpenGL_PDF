#include <iostream>
#include <vector>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

void ProcessInputs(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int LoadTexture(const char* path);

// Settings
unsigned int SCR_WIDTH = 900;
unsigned int SCR_HEIGHT = 900;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;
bool firstMouse = true;




int main()
{
	// glfw: initialize and configure
	// ------------------------------
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create window!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Setting up all glfw callback functions
	// --------------------------------------
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!\n" << glfwGetVersionString();
		return -1;
	}

	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// build and compile shaders
	// -------------------------
	Shader shader("src/res/shaders/vertex.vert", "src/res/shaders/fragment.frag");

	// set up cube vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	const float cubeVertices[] = {
		// positions			// texture coords
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,	1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,	1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,	1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,	1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,	1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,	1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,	0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,	0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,	1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,	1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,	1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,	1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f
	};
	const float planeVertices[] = {
		// positions          // texture Coords 
		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};
	const float windowPlaneVertices[] = {
		// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  0.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  0.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  1.0f
	};
	// Cube VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glBindVertexArray(cubeVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)) );
	// Plane VAO
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glBindVertexArray(planeVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	// Window plane VAO
	unsigned int windowPlaneVAO, windowPlaneVBO;
	glGenVertexArrays(1, &windowPlaneVAO);
	glGenBuffers(1, &windowPlaneVBO);
	glBindBuffer(GL_ARRAY_BUFFER, windowPlaneVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), windowPlaneVertices, GL_STATIC_DRAW);
	glBindVertexArray(windowPlaneVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	// List of window locations
	std::vector<glm::vec3> windowPositions
	{
		glm::vec3(-1.5f, 0.0f, -0.48f),
		glm::vec3(1.5f, 0.0f, 0.51f),
		glm::vec3(0.0f, 0.0f, 0.7f),
		glm::vec3(-0.3f, 0.0f, -2.3f),
		glm::vec3(0.5f, 0.0f, -0.6f)
	};

	// Load Textures
	// -------------
	unsigned int marbleTexture = LoadTexture("src/res/textures/marble.jpg");
	unsigned int metalTexture = LoadTexture("src/res/textures/metal.png");
	unsigned int windowTexture = LoadTexture("src/res/textures/window.png");

	std::map<float, glm::vec3> sortedWindows;
	for (unsigned int i = 0; i < windowPositions.size(); i++)
	{
		float distance = glm::length(camera.Position - windowPositions[i]);
		sortedWindows[distance] = windowPositions[i];
	}

	// Render Loop
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Inputs
		// ------
		ProcessInputs(window);

		// Render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Activate shader
		shader.Use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		shader.SetMat4("projection", projection);
		shader.SetMat4("view", view);

		// Plane
		glBindVertexArray(planeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, metalTexture);
		model = glm::mat4(1.0f);
		shader.SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Cubes
		glBindVertexArray(cubeVAO);
		glBindTexture(GL_TEXTURE_2D, marbleTexture);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		shader.SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		shader.SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Window
		glBindVertexArray(windowPlaneVAO);
		glBindTexture(GL_TEXTURE_2D, windowTexture);
		for (auto rit = sortedWindows.rbegin(); rit != sortedWindows.rend(); rit++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, rit->second);
			shader.SetMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void ProcessInputs(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessMovement(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessMovement(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessMovement(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessMovement(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessMovement(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.ProcessMovement(DOWN, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// Utility function for lading a 2D texture from file
// --------------------------------------------------
unsigned int LoadTexture(const char* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if      (nrComponents == 1) format = GL_RED;
		else if (nrComponents == 3) format = GL_RGB;
		else if (nrComponents == 4) format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		if (format == GL_RGBA)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		}
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		std::cout << "Failed to load at path: " << path << std::endl;
	}
	stbi_image_free(data);

	return textureID;
}