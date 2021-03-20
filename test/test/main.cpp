#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <iostream>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);

const unsigned int SRC_WIDTH = 800;
const unsigned int SRC_HEIGHT = 600;

float lastX = 400, lastY = 300;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow * window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "Test", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	unsigned int texture1, texture2;
	int width, height, nrChannels;
	unsigned char* data;
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);

	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("kosh.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("texture1.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view;
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);


	Shader shader("shader.vs", "shader.fs");

	glUseProgram(shader.getId());
	shader.setUniformInt("texture0", 0);
	shader.setUniformInt("texture1", 1);
	unsigned int modelLoc = glGetUniformLocation(shader.getId(), "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	unsigned int viewLoc = glGetUniformLocation(shader.getId(), "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	unsigned int projectionLoc = glGetUniformLocation(shader.getId(), "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	
	float vertices[] = {	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
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
							-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	
							//PLANE
							-1.0f,  -1.0f, -1.0f,  0.0f, 1.0f,
							 1.0f,  -1.0f, -1.0f,  1.0f, 1.0f,
							-1.0f,  -1.0f,  1.0f,  0.0f, 0.0f,
							 1.0f,  -1.0f, -1.0f,  1.0f, 1.0f,
							 1.0f,  -1.0f,  1.0f,  1.0f, 0.0f,
							-1.0f,  -1.0f,  1.0f,  0.0f, 0.0f,
							};

	unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3 };

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);


	double frameTime;
	double lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		frameTime = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();
		//std::cout << multAngle << "::" << (1-multAngle) << std::endl;

		const float cameraSpeed = 5.0f * frameTime; // настройте по вашему усмотрению
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			cameraPos += cameraUp * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			cameraPos -= cameraUp * cameraSpeed;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glUseProgram(shader.getId());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		int height, width;
		glfwGetFramebufferSize(window, &width, &height);
		projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		float multAngle = glm::abs((float)glm::sin(lastTime));
		model = glm::rotate(model, (1 - multAngle) * (float)frameTime * glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, multAngle * (float)frameTime * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		shader.setUniformInt("texture1", 1);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 modelPlane = glm::mat4(1.0f);
		modelPlane = glm::scale(modelPlane, glm::vec3(2.0f, 1.0f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPlane));
		shader.setUniformInt("texture1", 0);
		glDrawArrays(GL_TRIANGLES, 36, 6);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	float xOffset = lastX - xPos;
	float yOffset = yPos - yPos;
	lastX = xPos;
	lastY = yPos;

	const float sensitivity = 0.05f;

}