#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include "allmodels.h"
#include "constants.h"

using namespace glm;


float aspect = 1;
float speed = PI/4;
glm::vec3 cameraPosition;
glm::vec3 cameraCenterPosition;
float cameraRotateVerticalAngle;
float cameraRotateHorizontalAngle;
float cameraDistance;
float rotateAngle;


//Procedura obs�ugi b��d�w
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura zmieniaj�ca rozmiar ramki 
void windowResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height); //Generate images in this resolution
	aspect = (float)width / (float)height; //Compute aspect ratio of width to height of the window
}

//Procedura obliczaj�ca wsp�rz�dne kamery na podstawie k�t�w obrotu w dw�ch osiach i odleg�o�ci kamery od �rodka uk�adu 
void calculatePosition() {
	cameraPosition.x = cos((cameraRotateVerticalAngle * PI) / 180.0f) * sin((cameraRotateHorizontalAngle * PI) / 180.0f) * cameraDistance;
	cameraPosition.y = sin((cameraRotateVerticalAngle * PI) / 180.0f) * cameraDistance;
	cameraPosition.z = cos((cameraRotateVerticalAngle * PI) / 180.0f) * cos((cameraRotateHorizontalAngle * PI) / 180.0f) * cameraDistance;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS))
		cameraRotateVerticalAngle -= 1;
	if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS))
		cameraRotateVerticalAngle += 1;

	if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS))
		cameraRotateHorizontalAngle += 1;
	if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS))
		cameraRotateHorizontalAngle -= 1;
	
	if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
		cameraDistance += 10;
	if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
		cameraDistance -= 10;

	if (key == GLFW_KEY_Q && (action == GLFW_REPEAT || action == GLFW_PRESS))
		rotateAngle += PI / 4.0f;
	if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
		rotateAngle += PI / 4.0f;

	printf("%f\n", rotateAngle);

	calculatePosition();
}



//Procedura inicjuj�ca
void initOpenGLProgram(GLFWwindow* window) {

	glfwSetFramebufferSizeCallback(window, windowResize);  //Zarejestruj procedur� zmieniaj�c� rozmiar ramki
	glfwSetErrorCallback(error_callback);	//Zarejestruj procedur� obs�ugi b��d�w
	glfwSetKeyCallback(window, key_callback);  

	//glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

	glClearColor(0, 0, 0, 1); //Ustaw kolor okna po wyczyszczeniu
	glEnable(GL_LIGHTING); //W��cz model o�wietlenia
	glEnable(GL_LIGHT0); //W��cz o�wietlenie 0
	glEnable(GL_DEPTH_TEST); //W��cz depthbuffer
	glEnable(GL_COLOR_MATERIAL); //W��cz obs�ug� materia�u

	cameraDistance = -400.0f;
	cameraPosition = { 0.0f, 0.0f, -400.0f };
	cameraCenterPosition = { 0.0f, 0.0f, 0.0f };
	cameraRotateHorizontalAngle = 0;
	cameraRotateVerticalAngle = 0;
}

//Procedura rysuj�ca zawarto�� sceny
void drawScene(GLFWwindow* window) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczy�� buffer koloru i przygotuj do rysowania 

														//***Prepare to draw****
	mat4 P = perspective(50.0f*PI/180.0f, aspect, 1.0f, 800.0f); //Compute projection matrix
	mat4 V = lookAt(cameraPosition, cameraCenterPosition, vec3(0.0f, 1.0f, 0.0f)); //Compute view matrix
	glMatrixMode(GL_PROJECTION); //Turn on projection matrix editing mode
	glLoadMatrixf(value_ptr(P)); //Load projection matrix
	glMatrixMode(GL_MODELVIEW);  //Turn on modelview matrix editing mode

	mat4 M = mat4(1.0f);
	//M = rotate(M, 45*PI/180, vec3(0.0f, 1.0f, 0.0f));
	M = translate(M, vec3(0.0f, 90.0f, 0.0f));
	M = rotate(M, rotateAngle, vec3(0.0f, 1.0f, 0.0f));
	glLoadMatrixf(value_ptr(V*M));
	glColor3d(1.0f, 1.0f, 0.0f); //Set the objects color
	Models::piston.drawSolid(); //Draw the model

	M = mat4(1.0f);
	//M = rotate(M, 45*PI/180, vec3(0.0f, 1.0f, 0.0f));
	M = translate(M, vec3(0.0f, -90.0f, 0.0f));
	M = rotate(M, rotateAngle, vec3(0.0f, 1.0f, 0.0f));
	glLoadMatrixf(value_ptr(V*M));
	glColor3d(0.0f, 1.0f, 1.0f); //Set the objects color
	Models::conrod.drawSolid(); //Draw the model

	M = mat4(1.0f);
	//M = rotate(M, 45*PI/180, vec3(0.0f, 1.0f, 0.0f));
	M = translate(M, vec3(0.0f, 5.0f, 0.0f));
	M = rotate(M, rotateAngle, vec3(1.0f, 0.0f, 0.0f));
	glLoadMatrixf(value_ptr(V*M));
	glColor3d(1.0f, 0.0f, 1.0f); //Set the objects color
	Models::crankshaft.drawSolid(); //Draw the model


	glfwSwapBuffers(window); //Swap the back and front buffers
}

int main(void)
{
	GLFWwindow* window;						//Wska�nik na obiekt reprezentuj�cy okno

	if (!glfwInit()) {						//Zainicjuj bibliotek� GLFW
		fprintf(stderr, "Nie mo�na zainicjowa� GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utw�rz okno 500x500 o tytule "OpenGL" i kontekst OpenGL. 

	if (!window) {				
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);			//Od tego momentu kontekst okna staje si� aktywny i polecenia OpenGL b�d� dotyczy� w�a�nie jego.
	glfwSwapInterval(1);					//Czekaj na 1 powr�t plamki przed pokazaniem ukrytego bufora

	GLenum err;
	if ((err = glewInit()) != GLEW_OK) {	//Zainicjuj bibliotek� GLEW		
		fprintf(stderr, "Nie mo�na zainicjowa� GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	

	initOpenGLProgram(window); //Operacje inicjuj�ce

	rotateAngle = 0;
	float height = 0;
	int direction = 1;
	glfwSetTime(0);

	//G��wna p�tla
	while (!glfwWindowShouldClose(window))
	{
		//rotateAngle += speed * glfwGetTime();
		//printf("%f\n", rotateAngle);
		glfwSetTime(0);

		drawScene(window);	//Wykonaj procedur� rysuj�c�
		glfwPollEvents();	//Wykonaj procedury callback w zalezno�ci od zdarze� jakie zasz�y.
	}

	glfwDestroyWindow(window); //Usu� kontekst OpenGL i okno
	glfwTerminate(); 
	exit(EXIT_SUCCESS);
}
