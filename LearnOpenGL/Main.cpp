#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

float verticesBox[] = {
	// positions          // colors           // texture coords
	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
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

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	/*	SHADER STUFF	*/
	Shader shaders("Shaders\\TexturesLesson.vs",
		"Shaders\\TexturesLesson.fs");

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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

	// rendering loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// setting up clear screen stuff
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBox);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureFace);
		// draw stuff
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