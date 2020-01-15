#include <iostream>
#include <vector>

//GLEW
#include <GL\glew.h>

//GLFW
#include <GLFW\glfw3.h>

//GLM
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Shader.h"
#include "Loader.h"
#include "Mesh.h"

//Window dimension
const GLint WIDTH = 800, HEIGHT = 600;

int main()
{
	//Init GLFW
	glfwInit();

	//Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 16);

	//Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "STL loader example", nullptr, nullptr);

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (window == nullptr)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	//Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	//Initialize GLEW to setup OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		cout << "Failed to initialize GLEW" << endl;
		return EXIT_FAILURE;
	}

	//Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	glEnable(GL_DEPTH_TEST);

	//For multisampling
	glEnable(GL_MULTISAMPLE); 

	//Shaders support
	Shader ourShader("shaders/coreVertexShader.vert", "shaders/coreFragmentShader.frag");

	Loader loader;
	Mesh* mesh = loader.loadStl();
	if (mesh == nullptr)
	{
		cout << "Can't load .stl model." << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	std::vector<GLfloat> vertices = mesh->getVertices();
	glm::vec3 centerMesh = loader.setMeshCenter(mesh);

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0);  //Unbind VAO

	while (!glfwWindowShouldClose(window))
	{
		//Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//Render
		//Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Draw model
		ourShader.use();

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		model = glm::rotate(model, -30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));	
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::translate(model, -centerMesh);
		view = glm::lookAt(glm::vec3(0.0f, 40.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		projection = glm::perspective(45.0f, (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 1000.0f);

		GLint modelLoc = glGetUniformLocation(ourShader.program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.program, "view");
		GLint projectionLoc = glGetUniformLocation(ourShader.program, "projection");
		
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);

		//Swap the screen buffers
		glfwSwapBuffers(window);
	}

	//Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	delete mesh;

	//Terminate GLFW, clearing any resources allocated by GLFW
	glfwTerminate();

	return EXIT_SUCCESS;
}

