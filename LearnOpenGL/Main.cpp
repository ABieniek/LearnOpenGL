#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

// global stuff
double xPos = 0.0;
double yPos = 0.0;
double zPos = -3.0;
float xRot = 0.0;
float yRot = 0.0;
float zRot = 0.0;
float SCREEN_WIDTH = 800;
float SCREEN_HEIGHT = 600;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	// escape
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	// left
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		xPos -= .001;
	}
	// right
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		xPos += .001;
	}
	// down
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		yPos -= .001;
	}
	// up
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		yPos += .001;
	}
	// S
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		zPos -= .001;
	}
	// W
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		zPos += .001;
	}
	// LALT + DOWN
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		yRot -= .1f;
	}
	// LALT + UP
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		yRot += .1f;
	}
	// LALT + LEFT
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		xRot -= .1f;
	}
	// LALT + RIGHT
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		xRot += .1f;
	}
	// LALT + S
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		zRot -= .1f;
	}
	// LALT + W
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		zRot += .1f;
	}
}

float verticesBox[] = {
	// positions          // colors           // texture coords
	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // top right
	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // top left 
};

unsigned int indicesBox[] = {
	0, 1, 3,	// first triangle
	1, 2, 3		// second triangle
};

int main()
{
	/*	WINDOW STUFF	*/
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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	/*	SHADER STUFF	*/
	Shader shaders("Shaders\\CoordinateSystemsLesson.vs",
		"Shaders\\CoordinateSystemsLesson.fs");

	unsigned int VBOBox, VAOBox, EBOBox;
	glGenVertexArrays(1, &VAOBox);
	glGenBuffers(1, &VBOBox);
	glGenBuffers(1, &EBOBox);
	glBindVertexArray(VAOBox);

	glBindBuffer(GL_ARRAY_BUFFER, VBOBox);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBox), verticesBox, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOBox);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesBox), indicesBox, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	/*	TEXTURE STUFF	*/
	stbi_set_flip_vertically_on_load(true);
	// box
	unsigned int textureBox;
	glGenTextures(1, &textureBox);
	glBindTexture(GL_TEXTURE_2D, textureBox);
	//set texture wrapping and filtering options of textureBox
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// load image, creat texture, generate mipmaps
	int widthBox, heightBox, nrChannelsBox;
	unsigned char *dataBox = stbi_load("Textures\\container.png", &widthBox, &heightBox, &nrChannelsBox, 0);
	if (dataBox)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthBox, heightBox, 0, GL_RGB, GL_UNSIGNED_BYTE, dataBox);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	stbi_image_free(dataBox);

	// face
	unsigned int textureFace;
	glGenTextures(1, &textureFace);
	glBindTexture(GL_TEXTURE_2D, textureFace);
	//set texture wrapping and filtering options of textureFace
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, creat texture, generate mipmaps
	int widthFace, heightFace, nrChannelsFace;
	unsigned char *dataFace = stbi_load("Textures\\awesomeface.png", &widthFace, &heightFace, &nrChannelsFace, 0);
	if (dataFace)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthFace, heightFace, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataFace);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	stbi_image_free(dataFace);

	shaders.use();
	shaders.setInt("ourTexture0", 0);
	shaders.setInt("ourTexture1", 1);

	// transformation stuff
	glm::mat4 trans;
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
	trans = glm::rotate(trans, glm::degrees(90.0f), glm::vec3(0.0, 0.0, 1.0));


	unsigned int transformLoc = glGetUniformLocation(shaders.program, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	// rendering loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// changing the mix percentage through user input
		shaders.setFloat("mixPercent", .2);

		// setting up clear screen stuff
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBox);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureFace);

		// set up transformations
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		model = glm::rotate(model, glm::radians((float) yRot), glm::vec3(1.0f, 0.0f, 0.0f)); // the final argument determines axis of rotation, somehow
		model = glm::rotate(model, glm::radians((float) xRot), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians((float) zRot), glm::vec3(0.0f, 1.0f, 0.0f));
		// retrieve matrix uniform locations
		view = glm::translate(view, glm::vec3(xPos, yPos, zPos));
		projection = glm::perspective(glm::radians((float) -55.0), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		
		shaders.setMat4("model", model);
		shaders.setMat4("view", view);
		shaders.setMat4("projection", projection);

		glBindVertexArray(VAOBox);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAOBox);
	glDeleteBuffers(1, &VBOBox);
	glDeleteBuffers(1, &EBOBox);

	glfwTerminate();
	return 0;
}