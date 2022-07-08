// FirstProject1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <cmath>
#include <iostream>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaderProgram.h"
#include "stb_image.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "pyramid.h"
#include "cube.h"
#include "Camera.h"
#include "sphere.h"
#include "assimp/config.h"
#include "model.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//initialize the different fragment sources

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);


}

void mouse_callback(GLFWwindow* window, double Xpos, double Ypos) {
	float xpos = static_cast<float>(Xpos);
	float ypos = static_cast<float>(Ypos);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

;
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void loadTextures(unsigned int* textures) {
	glGenTextures(2, textures);

	glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	//texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Comment or uncomment if the picture appears upside-down
	stbi_set_flip_vertically_on_load(true);
	//The data that corresponds to our image.

	int width, height, nrChannels;
	unsigned char* data = stbi_load("container2.png", &width, &height, &nrChannels, 0);
	if (data) {
		//texture target,mipmap level,store rgb values,dimensions,always zero (legacy), format and data,actual data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "ERROR::STBI_LOAD::FAILED" << std::endl;
	}

	// free image memory
	stbi_image_free(data);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);

	data = stbi_load("container2_specular.png", &width, &height, &nrChannels, 0);
	if (data) {
		//texture target,mipmap level,store rgb values,dimensions,always zero (legacy), format and data,actual data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "ERROR::STBI_LOAD::FAILED" << std::endl;
	}

	// free image memory
	stbi_image_free(data);


	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textures[2]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);

	data = stbi_load("matrix.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		//texture target,mipmap level,store rgb values,dimensions,always zero (legacy), format and data,actual data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "ERROR::STBI_LOAD::FAILED" << std::endl;
	}

	// free image memory
	stbi_image_free(data);

}

void LoadBuffers(unsigned int* VBO, unsigned int* VAO, object& obj) {

	glGenVertexArrays(1, VAO);

	glGenBuffers(1, VBO);

	glBindVertexArray(*VAO);

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, obj.getSize(), obj.getVertices(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, obj.getStride(), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, obj.getStride(), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, obj.getStride(), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

}


int main()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;

		glViewport(0, 0, 800, 600);

		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	}

	Shader lightShader("vertex.vs", "fragment.fs");
	Shader lightCubeS("vertex.vs", "lightFragment.fs");

	//Pyramid pyr;
	//cube c1;
	sphere s;

	model ourModel("backpack/backpack.obj");



	//Make a unique buffer id and generate it 
	unsigned int VBO, VBO1, EBO, VAO, VAO1;

	//LoadBuffers(&VBO, &VAO, c1);

	glGenBuffers(1, &VBO1);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO1);

	glBindVertexArray(VAO1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, s.getVertSize(), s.getVertices(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, s.getIndSize(), s.getIndices(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, s.getStride(), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, s.getStride(), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, s.getStride(), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	//shader stuff

	glEnable(GL_DEPTH_TEST);

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);
		// rendering commands here

		//deltatime,time between the two frames. its used for the camera object.
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		float lightX = (float)sin(glfwGetTime());
		float lightY = 0.0f;
		float lightZ = cos(glfwGetTime());

		lightPos = glm::vec3(lightX, lightY, lightZ);

		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);

		lightShader.use();
		lightShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		lightShader.setVec3("lightPos", lightPos);
		lightShader.setVec3("viewPos", camera.Position);
		lightShader.setFloat("material.shininess", 32.0f);
		lightShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		lightShader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f)); // darken diffuse light a bit
		lightShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));


		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		lightShader.setMat("projection", projection);

		glm::mat4 view = camera.GetViewMatrix();
		lightShader.setMat("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.25f, 0.25f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		lightShader.setMat("model", model);
		ourModel.draw(lightShader);

		//a light sphere, basically a light source. Current light color is white.
		lightCubeS.use();
		lightCubeS.setMat("projection", projection);
		lightCubeS.setMat("view", view);

		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.05f));
		lightCubeS.setMat("model", lightModel);


		glBindVertexArray(VAO1);
		glDrawElements(GL_TRIANGLES, s.getIndCount(), GL_UNSIGNED_INT, (void*)0);

		// check and call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &VAO1);

	glfwTerminate();
	return 0;

}