#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>
#include <iostream>

//Procedura obs³ugi b³êdów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura inicjuj¹ca
void initOpenGLProgram() {

}

//Procedura rysuj¹ca zawartoœæ sceny
void drawScene(GLFWwindow* window) {

}

//janek

int main(void)
{
	GLFWwindow* window;						//WskaŸnik na obiekt reprezentuj¹cy okno

	glfwSetErrorCallback(error_callback);	//Zarejestruj procedurê obs³ugi b³êdów

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

	initOpenGLProgram(); //Operacje inicjuj¹ce
	//komentarz testowy 

	//G³ówna pêtla
	while (!glfwWindowShouldClose(window))
	{
		drawScene(window);	//Wykonaj procedurê rysuj¹c¹
		glfwPollEvents();	//Wykonaj procedury callback w zaleznoœci od zdarzeñ jakie zasz³y.
	}

	glfwDestroyWindow(window); //Usuñ kontekst OpenGL i okno
	glfwTerminate(); 
	exit(EXIT_SUCCESS);
}
