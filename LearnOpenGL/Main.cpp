#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

float verticesQuizTriangle1[] = {
	-0.75, -0.75, 0.0,
	-0.50, -0.75, 0.0,
	-0.50, +0.50, 0.0
};

float verticesQuizTriangle2[] = {
	-0.8, 0.0, 0.0,
	-0.1, +0.5, 0.0,
	+0.8, 0.0, 0.0
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform float fOffset;\n"
"out vec3 positionsToColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(-aPos.x + fOffset, -aPos.y, -aPos.z, 1.0);\n"
"	positionsToColor = vec3(-aPos.x, -aPos.y, -aPos.z);\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 positionsToColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(positionsToColor.xyz, 1.0f);\n"
"}\n\0";

const char *fragmentShaderSource1 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// compiling the vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
	glCompileShader(fragmentShader1);

	// checking if our compilation above succeeded
	// I'm too lazy to check if the second one succeeded, but I'm sure it would be similar
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	unsigned int shaderProgram1 = glCreateProgram();
	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragmentShader1);
	glLinkProgram(shaderProgram1);

	// checking if linking went well
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	}

	// we can now delete the shaders that we made
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader1);

	// generating VBO and VAO
	unsigned int VBOQuiz1, VAOQuiz1;
	glGenBuffers(1, &VBOQuiz1);
	glGenVertexArrays(1, &VAOQuiz1);

	unsigned int VBOQuiz2, VAOQuiz2;
	glGenBuffers(1, &VBOQuiz2);
	glGenVertexArrays(1, &VAOQuiz2);

	// bind VAO, set vertex buffers, configure vertex attributes
	glBindVertexArray(VAOQuiz1);
	glBindBuffer(GL_ARRAY_BUFFER, VBOQuiz1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesQuizTriangle1), 
		verticesQuizTriangle1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAOQuiz2);
	glBindBuffer(GL_ARRAY_BUFFER, VBOQuiz2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesQuizTriangle2),
		verticesQuizTriangle2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// rendering loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// setting up clear screen stuff
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// get the location of our uniform in the GPU
		int vertexOffsetLocation = glGetUniformLocation(shaderProgram, "fOffset");
		// use the created shaders
		glUseProgram(shaderProgram);
		// set the value of the uniform
		glUniform1f(vertexOffsetLocation, .3f); // just throwing in .5 as a random value
		// bind the VAO
		glBindVertexArray(VAOQuiz1);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// same as above
		glUseProgram(shaderProgram1);
		glBindVertexArray(VAOQuiz2);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}