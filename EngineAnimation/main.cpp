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

float rotateAngle;							//K¹t obrotu wa³u
float r;									//Promieñ wa³u (pó³ jednego suwu)
float l;									//D³ugoœæ korbowodu
float xpos0, xpos1, xpos2;					//Pozycja t³oka (wysokoœæ od œrodka wa³u)
float yoff0, yoff1, yoff2;					//Offset panweki w y
float zoff0, zoff1, zoff2;					//Offset panewki w z
float rodAngle0, rodAngle1, rodAngle2;		//K¹t odchylenia korbowodu
bool idle, rev;								//Automatyczny obrót silnika
float offset0 = 17.0f / 32.0f * PI, offset2 = -15.0f / 32.0f * PI;

//Procedura obs³ugi b³êdów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura zmieniaj¹ca rozmiar ramki 
void windowResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height); //Generate images in this resolution
	aspect = (float)width / (float)height; //Compute aspect ratio of width to height of the window
}

//Procedura obliczaj¹ca wspó³rzêdne kamery na podstawie k¹tów obrotu w dwóch osiach i odleg³oœci kamery od œrodka uk³adu 
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

	printf("%d/32 PI\n", int(rotateAngle/PI*32));	//¯eby ³atwiej by³o zrobiæ prawid³owy timing
	if (rotateAngle >= PI)
		rotateAngle = -PI;
	else if (rotateAngle <= -PI)
		rotateAngle = PI;

	calculatePosition();
}

//Procedura inicjuj¹ca
void initOpenGLProgram(GLFWwindow* window) {

	glfwSetFramebufferSizeCallback(window, windowResize);  //Zarejestruj procedurê zmieniaj¹c¹ rozmiar ramki
	glfwSetErrorCallback(error_callback);	//Zarejestruj procedurê obs³ugi b³êdów
	glfwSetKeyCallback(window, key_callback);  

	//glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

	glClearColor(0, 0, 0, 1); //Ustaw kolor okna po wyczyszczeniu
	glEnable(GL_LIGHTING); //W³¹cz model oœwietlenia
	glEnable(GL_LIGHT0); //W³¹cz oœwietlenie 0
	glEnable(GL_DEPTH_TEST); //W³¹cz depthbuffer
	glEnable(GL_COLOR_MATERIAL); //W³¹cz obs³ugê materia³u

	cameraDistance = -400.0f;
	cameraPosition = { 0.0f, 0.0f, -400.0f };
	cameraCenterPosition = { 0.0f, 0.0f, 0.0f };
	cameraRotateHorizontalAngle = 0;
	cameraRotateVerticalAngle = 0;
}

//Procedura rysuj¹ca zawartoœæ sceny
void drawScene(GLFWwindow* window) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyœæ buffer koloru i przygotuj do rysowania 

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
	xpos1 = r * cos(rotateAngle) + sqrt(l*l - (r*r*sin(rotateAngle)*sin(rotateAngle)));	//Wyliczanie pozycji t³oka
	yoff1 = r * cos(rotateAngle);	//Wyliczanie pozycji korbowodu
	zoff1 = r * sin(rotateAngle);	//Wyliczanie pozycji korbowodu
	rodAngle1 = acos(zoff1 / l);	//Wyliczanie k¹ta odchylenia korbowodu
	xpos2 = r * cos(rotateAngle - offset2) + sqrt(l*l - (r*r*sin(rotateAngle - offset2)*sin(rotateAngle - offset2)));
	yoff2 = r * cos(rotateAngle - offset2);
	zoff2 = r * sin(rotateAngle - offset2);
	rodAngle2 = acos(zoff2 / l);


	//T³ok 0.
	mat4 M = mat4(1.0f);
	M = translate(M, vec3(92.0f, 5.0f, 2.0f));			//Pozycja pocz¹tkowa
	M = translate(M, vec3(0.0f, xpos0, 0.0f));			//Ruch w górê i w dó³ zale¿ny od k¹ta obrotu wa³u
	glLoadMatrixf(value_ptr(V*M));
	glColor3d(1.0f, 1.0f, 0.0f);
	Models::piston.drawSolid();

	//T³ok 1.
	M = mat4(1.0f);
	M = translate(M, vec3(3.0f, 5.0f, 2.0f));			//Pozycja pocz¹tkowa
	M = translate(M, vec3(0.0f, xpos1, 0.0f));			//Ruch w górê i w dó³ zale¿ny od k¹ta obrotu wa³u
	glLoadMatrixf(value_ptr(V*M));
	glColor3d(1.0f, 1.0f, 0.0f); 
	Models::piston.drawSolid(); 

	//T³ok 2.
	M = mat4(1.0f);
	M = translate(M, vec3(-88.0f, 5.0f, 2.0f));			//Pozycja pocz¹tkowa
	M = translate(M, vec3(0.0f, xpos2, 0.0f));			//Ruch w górê i w dó³ zale¿ny od k¹ta obrotu wa³u
	glLoadMatrixf(value_ptr(V*M));
	glColor3d(1.0f, 1.0f, 0.0f);
	Models::piston.drawSolid();


	//Korobowód 0.
	M = mat4(1.0f);
	M = translate(M, vec3(92.0f, 2.0f, 2.0f));							//Pozycja pocz¹tkowa
	M = translate(M, vec3(0.0f, yoff0, zoff0));							//Ruch korbowodu
	M = rotate(M, 0.5f * PI, vec3(1.0f, 0.0f, 0.0f));					//Pozycja pocz¹tkowa
	M = rotate(M, rodAngle0, vec3(1.0f, 0.0f, 0.0f));					//Rotacja korbowodu 
	glLoadMatrixf(value_ptr(V*M));
	glColor3d(0.0f, 1.0f, 1.0f);
	Models::conrod.drawSolid();

	//Korobowód 1.
	M = mat4(1.0f); 
	M = translate(M, vec3(3.0f, 2.0f, 2.0f));							//Pozycja pocz¹tkowa
	M = translate(M, vec3(0.0f, yoff1, zoff1));							//Ruch korbowodu
	M = rotate(M, 0.5f * PI, vec3(1.0f, 0.0f, 0.0f));					//Pozycja pocz¹tkowa
	M = rotate(M, rodAngle1, vec3(1.0f, 0.0f, 0.0f));					//Rotacja korbowodu 
	glLoadMatrixf(value_ptr(V*M));
	glColor3d(0.0f, 1.0f, 1.0f); 
	Models::conrod.drawSolid(); 

	//Korobowód 2.
	M = mat4(1.0f);
	M = translate(M, vec3(-88.0f, 2.0f, 2.0f));							//Pozycja pocz¹tkowa
	M = translate(M, vec3(0.0f, yoff2, zoff2));							//Ruch korbowodu
	M = rotate(M, 0.5f * PI, vec3(1.0f, 0.0f, 0.0f));					//Pozycja pocz¹tkowa
	M = rotate(M, rodAngle2, vec3(1.0f, 0.0f, 0.0f));					//Rotacja korbowodu 
	glLoadMatrixf(value_ptr(V*M));
	glColor3d(0.0f, 1.0f, 1.0f);
	Models::conrod.drawSolid();

	M = mat4(1.0f);
	M = rotate(M, -(6.0f / 32.0f)*PI, vec3(1.0f, 0.0f, 0.0f));	//Ustawienie pozycji pocz¹tkowej wa³u w celu synchronizacji 
	M = rotate(M, rotateAngle, vec3(1.0f, 0.0f, 0.0f));
	glLoadMatrixf(value_ptr(V*M));
	glColor3d(1.0f, 0.0f, 1.0f); 
	Models::crankshaft.drawSolid(); 

	glfwSwapBuffers(window); //Swap the back and front buffers
}

int main(void)
{
	GLFWwindow* window;						//WskaŸnik na obiekt reprezentuj¹cy okno

	if (!glfwInit()) {						//Zainicjuj bibliotekê GLFW
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL. 

	if (!window) {				
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);			//Od tego momentu kontekst okna staje siê aktywny i polecenia OpenGL bêd¹ dotyczyæ w³aœnie jego.
	glfwSwapInterval(1);					//Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	GLenum err;
	if ((err = glewInit()) != GLEW_OK) {	//Zainicjuj bibliotekê GLEW		
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjuj¹ce

	//Orientacyjne wartoœci - do zmierzenia
	r = 25.0f;
	l = 80.0f;

	idle = false;
	rotateAngle = 0;
	float height = 0;
	int direction = 1;
	glfwSetTime(0);

	//G³ówna pêtla
	while (!glfwWindowShouldClose(window))
	{
		glfwSetTime(0);

		drawScene(window);	//Wykonaj procedurê rysuj¹c¹
		glfwPollEvents();	//Wykonaj procedury callback w zaleznoœci od zdarzeñ jakie zasz³y.
	}

	glfwDestroyWindow(window); //Usuñ kontekst OpenGL i okno
	glfwTerminate(); 
	exit(EXIT_SUCCESS);
}
