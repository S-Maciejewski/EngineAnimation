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

float rotateAngle;							//K�t obrotu wa�u
float r;									//Promie� wa�u (p� jednego suwu)
float l;									//D�ugo�� korbowodu
float xpos0, xpos1, xpos2;					//Pozycja t�oka (wysoko�� od �rodka wa�u)
float yoff0, yoff1, yoff2;					//Offset panweki w y
float zoff0, zoff1, zoff2;					//Offset panewki w z
float rodAngle0, rodAngle1, rodAngle2;		//K�t odchylenia korbowodu
bool idle, rev;								//Automatyczny obr�t silnika
float offset0 = 17.0f / 32.0f * PI, offset2 = -15.0f / 32.0f * PI;

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

	if (key == GLFW_KEY_T && action == GLFW_PRESS)
		idle = !idle;
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
		rev = !rev;

	if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS))
		cameraRotateVerticalAngle -= 5;
	if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS))
		cameraRotateVerticalAngle += 5;

	if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS))
		cameraRotateHorizontalAngle -= 5;
	if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS))
		cameraRotateHorizontalAngle += 5;
	
	if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
		cameraDistance += 10;
	if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
		cameraDistance -= 10;

	if (key == GLFW_KEY_Q && (action == GLFW_REPEAT || action == GLFW_PRESS))
		rotateAngle += PI / 32.0f;
	if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
		rotateAngle -= PI / 32.0f;

	printf("%d/32 PI\n", int(rotateAngle/PI*32));	//�eby �atwiej by�o zrobi� prawid�owy timing
	if (rotateAngle >= PI)
		rotateAngle = -PI;
	else if (rotateAngle <= -PI)
		rotateAngle = PI;

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

	mat4 P = perspective(50.0f*PI/180.0f, aspect, 1.0f, 800.0f); //Compute projection matrix
	mat4 V = lookAt(cameraPosition, cameraCenterPosition, vec3(0.0f, 1.0f, 0.0f)); //Compute view matrix
	glMatrixMode(GL_PROJECTION); //Turn on projection matrix editing mode
	glLoadMatrixf(value_ptr(P)); //Load projection matrix
	glMatrixMode(GL_MODELVIEW);  //Turn on modelview matrix editing mode

	if (idle)
		rotateAngle += PI / 32.0f;
	if (rev)
		rotateAngle += PI / 8.0f;

	if (rotateAngle >= PI)
		rotateAngle = -PI;
	else if (rotateAngle <= -PI)
		rotateAngle = PI;

	//Obliczenia kinematyczne
	xpos0 = r * cos(rotateAngle - offset0) + sqrt(l*l - (r*r*sin(rotateAngle - offset0)*sin(rotateAngle - offset0)));
	yoff0 = r * cos(rotateAngle - offset0);
	zoff0 = r * sin(rotateAngle - offset0);
	rodAngle0 = acos(zoff0 / l);
	xpos1 = r * cos(rotateAngle) + sqrt(l*l - (r*r*sin(rotateAngle)*sin(rotateAngle)));	//Wyliczanie pozycji t�oka
	yoff1 = r * cos(rotateAngle);	//Wyliczanie pozycji korbowodu
	zoff1 = r * sin(rotateAngle);	//Wyliczanie pozycji korbowodu
	rodAngle1 = acos(zoff1 / l);	//Wyliczanie k�ta odchylenia korbowodu
	xpos2 = r * cos(rotateAngle - offset2) + sqrt(l*l - (r*r*sin(rotateAngle - offset2)*sin(rotateAngle - offset2)));
	yoff2 = r * cos(rotateAngle - offset2);
	zoff2 = r * sin(rotateAngle - offset2);
	rodAngle2 = acos(zoff2 / l);


	//T�ok 0.
	mat4 M = mat4(1.0f);
	M = translate(M, vec3(92.0f, 5.0f, 2.0f));			//Pozycja pocz�tkowa
	M = translate(M, vec3(0.0f, xpos0, 0.0f));			//Ruch w g�r� i w d� zale�ny od k�ta obrotu wa�u
	glLoadMatrixf(value_ptr(V*M));
	glColor3d(1.0f, 1.0f, 0.0f);
	Models::piston.drawSolid();

	//T�ok 1.
	M = mat4(1.0f);
	M = translate(M, vec3(3.0f, 5.0f, 2.0f));			//Pozycja pocz�tkowa
	M = translate(M, vec3(0.0f, xpos1, 0.0f));			//Ruch w g�r� i w d� zale�ny od k�ta obrotu wa�u
	glLoadMatrixf(value_ptr(V*M));
	glColor3d(1.0f, 1.0f, 0.0f); 
	Models::piston.drawSolid(); 

	//T�ok 2.
	M = mat4(1.0f);
	M = translate(M, vec3(-88.0f, 5.0f, 2.0f));			//Pozycja pocz�tkowa
	M = translate(M, vec3(0.0f, xpos2, 0.0f));			//Ruch w g�r� i w d� zale�ny od k�ta obrotu wa�u
	glLoadMatrixf(value_ptr(V*M));
	glColor3d(1.0f, 1.0f, 0.0f);
	Models::piston.drawSolid();


	//Korobow�d 0.
	M = mat4(1.0f);
	M = translate(M, vec3(92.0f, 2.0f, 2.0f));							//Pozycja pocz�tkowa
	M = translate(M, vec3(0.0f, yoff0, zoff0));							//Ruch korbowodu
	M = rotate(M, 0.5f * PI, vec3(1.0f, 0.0f, 0.0f));					//Pozycja pocz�tkowa
	M = rotate(M, rodAngle0, vec3(1.0f, 0.0f, 0.0f));					//Rotacja korbowodu 
	glLoadMatrixf(value_ptr(V*M));
	glColor3d(0.0f, 1.0f, 1.0f);
	Models::conrod.drawSolid();

	//Korobow�d 1.
	M = mat4(1.0f); 
	M = translate(M, vec3(3.0f, 2.0f, 2.0f));							//Pozycja pocz�tkowa
	M = translate(M, vec3(0.0f, yoff1, zoff1));							//Ruch korbowodu
	M = rotate(M, 0.5f * PI, vec3(1.0f, 0.0f, 0.0f));					//Pozycja pocz�tkowa
	M = rotate(M, rodAngle1, vec3(1.0f, 0.0f, 0.0f));					//Rotacja korbowodu 
	glLoadMatrixf(value_ptr(V*M));
	glColor3d(0.0f, 1.0f, 1.0f); 
	Models::conrod.drawSolid(); 

	//Korobow�d 2.
	M = mat4(1.0f);
	M = translate(M, vec3(-88.0f, 2.0f, 2.0f));							//Pozycja pocz�tkowa
	M = translate(M, vec3(0.0f, yoff2, zoff2));							//Ruch korbowodu
	M = rotate(M, 0.5f * PI, vec3(1.0f, 0.0f, 0.0f));					//Pozycja pocz�tkowa
	M = rotate(M, rodAngle2, vec3(1.0f, 0.0f, 0.0f));					//Rotacja korbowodu 
	glLoadMatrixf(value_ptr(V*M));
	glColor3d(0.0f, 1.0f, 1.0f);
	Models::conrod.drawSolid();

	M = mat4(1.0f);
	M = rotate(M, -(6.0f / 32.0f)*PI, vec3(1.0f, 0.0f, 0.0f));	//Ustawienie pozycji pocz�tkowej wa�u w celu synchronizacji 
	M = rotate(M, rotateAngle, vec3(1.0f, 0.0f, 0.0f));
	glLoadMatrixf(value_ptr(V*M));
	glColor3d(1.0f, 0.0f, 1.0f); 
	Models::crankshaft.drawSolid(); 

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

	//Orientacyjne warto�ci - do zmierzenia
	r = 25.0f;
	l = 80.0f;

	idle = false;
	rotateAngle = 0;
	float height = 0;
	int direction = 1;
	glfwSetTime(0);

	//G��wna p�tla
	while (!glfwWindowShouldClose(window))
	{
		glfwSetTime(0);

		drawScene(window);	//Wykonaj procedur� rysuj�c�
		glfwPollEvents();	//Wykonaj procedury callback w zalezno�ci od zdarze� jakie zasz�y.
	}

	glfwDestroyWindow(window); //Usu� kontekst OpenGL i okno
	glfwTerminate(); 
	exit(EXIT_SUCCESS);
}
