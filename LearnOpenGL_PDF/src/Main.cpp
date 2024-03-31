#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"

void ProcessInputs(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

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

// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
const float vertices[] = {
	//		Positions			   Normals
	-0.5f, -0.5f, -0.5f,	  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,	  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,	  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,	  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,	  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,	  0.0f,  0.0f, -1.0f,
							
	-0.5f, -0.5f,  0.5f,	  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,	  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,	  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,	  0.0f,  0.0f, 1.0f,
							
	-0.5f,  0.5f,  0.5f,	 -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,	 -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,	 -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,	 -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,	 -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,	 -1.0f,  0.0f,  0.0f,
							
	 0.5f,  0.5f,  0.5f,	  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,	  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,	  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,	  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,	  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,	  1.0f,  0.0f,  0.0f,
							
	-0.5f, -0.5f, -0.5f,	  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,	  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,	  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,	  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,	  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,	  0.0f, -1.0f,  0.0f,
							
	-0.5f,  0.5f, -0.5f,	  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,	  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,	  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,	  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,	  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,	  0.0f,  1.0f,  0.0f
};
// world space positions of our cubes
glm::vec3 cubePositions[] = {
	 glm::vec3(0.0f, 0.0f, 0.0f),
	 glm::vec3(1.2f, 1.0f, 2.0f)
};


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
	GLFWwindow *window = glfwCreateWindow(SCR_HEIGHT, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create window!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

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

	// Vertex Array Object, Vertex Buffer Object
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Light VAO
	// ---------
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);

	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);


	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);

	// load and create a texture 
	// -------------------------
	unsigned int texture1, texture2;

	// Texture1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, noChannels;
	unsigned char* data = stbi_load("src/res/textures/container.jpg", &width, &height, &noChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(texture1);
	}
	else std::cout << "Failed to load the Texture!\n";

	// Texture2
	// ---------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("src/res/textures/awesomeface.png", &width, &height, &noChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(texture2);
	}
	else std::cout << "Failed to load the Texture!\n";

	stbi_image_free(data);

	Shader shader("src/res/shaders/vertex.vert", "src/res/shaders/fragment.frag");
	Shader lightShader("src/res/shaders/vertex.vert", "src/res/shaders/lightFragment.frag");

	shader.Use();
	shader.SetInt("texture1", 0);
	shader.SetInt("texture2", 1);

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

		// Bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// Activate shader
		shader.Use();

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH/SCR_HEIGHT, 0.1f, 100.0f);
		shader.SetMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		shader.SetMat4("view", view);

		// model transformation
		glm::mat4 model = glm::mat4(1.0f);
		shader.SetMat4("model", model);

		shader.SetVec3("objectColor", 1.0f, 0.5f, 0.31f);
		shader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
		shader.SetVec3("lightPos", 1.2f, 1.0f, 2.0f);

		// Render Box
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Activate shader
		lightShader.Use();

		lightShader.SetMat4("projection", projection);
		lightShader.SetMat4("view", view);

		// model transformation
		model = glm::translate(glm::mat4(1.0f), cubePositions[1]);
		model = glm::scale(model, glm::vec3(0.2f));
		lightShader.SetMat4("model", model);

		// Render Box
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);



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

