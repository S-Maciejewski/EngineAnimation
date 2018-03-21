#include <GL\glew.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>

#include <iostream>

//Procedura obs³ugi b³êdów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura inicjuj¹ca
void initOpenGLProgram() {
	//************Tutaj umieszczaj kod, który nale¿y wykonaæ raz, na pocz¹tku programu************

}

//Procedura rysuj¹ca zawartoœæ sceny
void drawScene(GLFWwindow* window) {
	//************Tutaj umieszczaj kod rysuj¹cy obraz******************
}

	int main(void)
	{
		GLFWwindow* window; //WskaŸnik na obiekt reprezentuj¹cy okno

		glfwSetErrorCallback(error_callback);//Zarejestruj procedurê obs³ugi b³êdów

		if (!glfwInit()) { //Zainicjuj bibliotekê GLFW
			fprintf(stderr, "Nie mo¿na zainicjowaæ GLFW.\n");
			exit(EXIT_FAILURE);
		}

		window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL. 

		if (!window) //Je¿eli okna nie uda³o siê utworzyæ, to zamknij program
		{
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje siê aktywny i polecenia OpenGL bêd¹ dotyczyæ w³aœnie jego.
		glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

		GLenum err;
		if ((err = glewInit()) != GLEW_OK) { //Zainicjuj bibliotekê GLEW		
			fprintf(stderr, "Nie mo¿na zainicjowaæ GLEW: %s\n", glewGetErrorString(err));
			exit(EXIT_FAILURE);
		}

		initOpenGLProgram(); //Operacje inicjuj¹ce

							 //G³ówna pêtla
		while (!glfwWindowShouldClose(window)) //Tak d³ugo jak okno nie powinno zostaæ zamkniête
		{
			drawScene(window); //Wykonaj procedurê rysuj¹c¹
			glfwPollEvents(); //Wykonaj procedury callback w zaleznoœci od zdarzeñ jakie zasz³y.
		}

		glfwDestroyWindow(window); //Usuñ kontekst OpenGL i okno
		glfwTerminate(); //Zwolnij zasoby zajête przez GLFW
		exit(EXIT_SUCCESS);
	}
