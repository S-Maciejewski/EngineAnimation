#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "allmodels.h"
#include "constants.h"

using namespace glm;

float aspect = 1;
float speed = PI/4;

//Error handling procedure
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Window resize procedure
void windowResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);		//Generate images in this resolution
	aspect = (float)width / (float)height;	//Compute aspect ratio of width to height of the window
}

//Procedure handling input events 
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//Initialization procedure
void initOpenGLProgram(GLFWwindow* window) {
	glfwSetFramebufferSizeCallback(window, windowResize);  //Register frame buffer resize procedure

	glClearColor(0, 0, 0, 1);	//Set clear background color
	glEnable(GL_LIGHTING);		//Turn on default lighting model
	glEnable(GL_LIGHT0);		//Turn on light 0
	glEnable(GL_DEPTH_TEST);	//Trun on depth buffer testing
	glEnable(GL_COLOR_MATERIAL);//Turn on glColor procedures for materials
}

//Main drawing procedure
void drawScene(GLFWwindow* window, float angle, float height) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				//Clear color buffer
												
	mat4 P = perspective(50.0f*PI / 180.0f, aspect, 1.0f, 50.0f);	//Compute projection matrix
	mat4 V = lookAt(												//Compute view matrix
		vec3(0.0f, 0.0f, -5.0f),
		vec3(0.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f));

	glMatrixMode(GL_PROJECTION); //Turn on projection matrix editing mode
	glLoadMatrixf(value_ptr(P)); //Load projection matrix
	glMatrixMode(GL_MODELVIEW);	 //Turn on modelview matrix editing mode
	mat4 M = mat4(1.0f);
	M = rotate(M, 45*PI/180, vec3(0.0f, 1.0f, 0.0f));
	M = translate(M, vec3(0.0f, height, 0.0f));
	M = rotate(M, angle, vec3(0.0f, 1.0f, 0.0f));
	glLoadMatrixf(value_ptr(V*M));

	glColor3d(1.0f, 1.0f, 0.0f); //Set the objects color
	Models::cube.drawWire();	 //Call the model drawing procedure

	glfwSwapBuffers(window);	 //Swap the back and front buffers
}

int main(void)
{
	GLFWwindow* window;						//Window object pointer

	glfwSetErrorCallback(error_callback);	//Register error processing procedure

	if (!glfwInit()) {						//GLFW library initialization
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Create window of certain size with "OpenGL" title

	if (!window) {	//Check if window was created succesfully		
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);			//Enable OpenGL context - OpenGL procedures will now affect given window
	glfwSwapInterval(1);					//Wait for the first refresh

	GLenum err;
	if ((err = glewInit()) != GLEW_OK) {	//GLEW library initialization	
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window);				//Call initialization procedure

	float angle = 0;
	float height = 0;
	int direction = 1;
	glfwSetTime(0);

	//Main loop
	while (!glfwWindowShouldClose(window))
	{

		processInput(window);   //Call input handling procedure

		angle += speed * glfwGetTime();
		if(direction == 1) height += speed * glfwGetTime();
		else height -= speed * glfwGetTime();

		if (height > 1) direction = -1;
		if (height < -1) direction = 1;
		
		glfwSetTime(0);

		drawScene(window, angle, height);	//Call drawing procedure
		glfwPollEvents();					//Process callback procedures corresponding to the events that took place up to now
	}

	glfwDestroyWindow(window);	//Delete OpenGL context and window
	glfwTerminate();			//Terminate GLFW process
	exit(EXIT_SUCCESS);
}
