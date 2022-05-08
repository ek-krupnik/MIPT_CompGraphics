
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
using namespace glm;

#include <common/shader.hpp>

void drawTriangle(GLuint programID, GLuint vertexbuffer) {
	glUseProgram(programID); // Use our shader

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
	glDisableVertexAttribArray(0);
}

int main( void )
{
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow( 1024, 768, "Two treangles", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.0f, 0.0f, 0.1f, 0.0f); // background color

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID1 = LoadShaders("SimpleVertexShader.vertexshader", "PurpleFragmentShader.fragmentshader" );
	GLuint programID2 = LoadShaders("SimpleVertexShader.vertexshader", "LightblueFragmentShader.fragmentshader");


	static const GLfloat triangle1[] = { 
		-0.5f, -0.4f, 0.0f,
		 0.3f, 0.0f, 0.0f,
		 0.0f,  0.2f, 0.0f,
	};

	static const GLfloat triangle2[] = {
		0.8f, 0.3f, 0.0f,
		 -0.1f, 0.0f, 0.0f,
		 0.0f,  -0.7f, 0.0f,
	};

	GLuint vertexbuffer[2];
	glGenBuffers(2, &vertexbuffer[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);

	do{

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		drawTriangle(programID1, vertexbuffer[0]);

		drawTriangle(programID2, vertexbuffer[1]);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO
	glDeleteBuffers(2, &vertexbuffer[0]);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID1);
	glDeleteProgram(programID2);

	glfwTerminate();

	return 0;
}

