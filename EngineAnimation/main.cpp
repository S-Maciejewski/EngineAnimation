#include <GL\glew.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>

#include <iostream>

//Procedura obs�ugi b��d�w
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura inicjuj�ca
void initOpenGLProgram() {
	//************Tutaj umieszczaj kod, kt�ry nale�y wykona� raz, na pocz�tku programu************

}

//Procedura rysuj�ca zawarto�� sceny
void drawScene(GLFWwindow* window) {
	//************Tutaj umieszczaj kod rysuj�cy obraz******************
}

	int main(void)
	{
		GLFWwindow* window; //Wska�nik na obiekt reprezentuj�cy okno

		glfwSetErrorCallback(error_callback);//Zarejestruj procedur� obs�ugi b��d�w

		if (!glfwInit()) { //Zainicjuj bibliotek� GLFW
			fprintf(stderr, "Nie mo�na zainicjowa� GLFW.\n");
			exit(EXIT_FAILURE);
		}

		window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utw�rz okno 500x500 o tytule "OpenGL" i kontekst OpenGL. 

		if (!window) //Je�eli okna nie uda�o si� utworzy�, to zamknij program
		{
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje si� aktywny i polecenia OpenGL b�d� dotyczy� w�a�nie jego.
		glfwSwapInterval(1); //Czekaj na 1 powr�t plamki przed pokazaniem ukrytego bufora

		GLenum err;
		if ((err = glewInit()) != GLEW_OK) { //Zainicjuj bibliotek� GLEW		
			fprintf(stderr, "Nie mo�na zainicjowa� GLEW: %s\n", glewGetErrorString(err));
			exit(EXIT_FAILURE);
		}

		initOpenGLProgram(); //Operacje inicjuj�ce

							 //G��wna p�tla
		while (!glfwWindowShouldClose(window)) //Tak d�ugo jak okno nie powinno zosta� zamkni�te
		{
			drawScene(window); //Wykonaj procedur� rysuj�c�
			glfwPollEvents(); //Wykonaj procedury callback w zalezno�ci od zdarze� jakie zasz�y.
		}

		glfwDestroyWindow(window); //Usu� kontekst OpenGL i okno
		glfwTerminate(); //Zwolnij zasoby zaj�te przez GLFW
		exit(EXIT_SUCCESS);
	}
