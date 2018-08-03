#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <fstream>

#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <SOIL.h>

// Includes GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#ifndef BUFFER_OFFSET

#define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

#endif

#include "Controller.h"

using namespace std;
using namespace glm;

void envoyerMat4(string nom, GLuint programID, glm::mat4 matrice)
{

	int localisation = glGetUniformLocation(programID, nom.c_str());

	glUniformMatrix4fv(localisation, 1, GL_FALSE, glm::value_ptr(matrice));

}

void envoyerVec2(std::string nom, GLuint programID, glm::vec2 vecteur)
{

	// Localisation de la matrice
	int localisation = glGetUniformLocation(programID, nom.c_str());

	// Envoi des valeurs
	glUniform2f(localisation, vecteur.x, vecteur.y);

}


void envoyer1I(std::string nom, GLuint programID, GLuint id)
{

	// Localisation de la matrice
	int localisation = glGetUniformLocation(programID, nom.c_str());

	// Envoi des valeurs
	glUniform1i(localisation, id);

}


void envoyerFloat(std::string nom, GLuint programID, float val)
{

	// Localisation de la matrice
	int localisation = glGetUniformLocation(programID, nom.c_str());

	// Envoi des valeurs
	glUniform1f(localisation, val);

}

bool compilerShader(GLuint &shader, GLenum type, string const &fichierSource)
{

	shader = glCreateShader(type);

	if (shader == 0)
	{
		std::cout << "Erreur, le type de shader (" << type << ") n'existe pas" << std::endl;
		return false;
	}

	ifstream fichier(fichierSource.c_str());

	if (!fichier)
	{
		std::cout << "Erreur le fichier " << fichierSource << " est introuvable" << std::endl;
		glDeleteShader(shader);

		return false;
	}

	std::string ligne;
	std::string codeSource;

	while (getline(fichier, ligne))
		codeSource += ligne + '\n';

	fichier.close();

	const GLchar* chaineCodeSource = codeSource.c_str();

	glShaderSource(shader, 1, &chaineCodeSource, 0);

	glCompileShader(shader);

	GLint erreurCompilation(0);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &erreurCompilation);

	if (erreurCompilation != GL_TRUE)
	{

		GLint tailleErreur(0);
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &tailleErreur);

		char *erreur = new char[tailleErreur + 1];

		glGetShaderInfoLog(shader, tailleErreur, &tailleErreur, erreur);
		erreur[tailleErreur] = '\0';

		std::cout << erreur << std::endl;

		delete[] erreur;
		glDeleteShader(shader);

		return false;
	}

	else
		return true;

}


int main(int argc, char **argv) {

    int err = glfwInit();
	if (!err) {

		std::cout << "Failed to initialize GLFW: " << err << "\n";
		exit(EXIT_FAILURE);

	}

	//=====================

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    int height = mode->height;
    int width = mode->width;
    std::cout << "Screen resolution: " << width << "x" << height << std::endl;

    GLFWwindow* window = glfwCreateWindow(width, height, "Test Mandelbrot", NULL, NULL);
	if (!window)
	{

		std::cout << "Erreur lors de la creation de la fenetre" << std::endl;

		glfwTerminate();
		exit(EXIT_FAILURE);

	}

	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

	//=====================

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1); //OpenGL 1.2
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	// On initialise GLEW
	GLenum initialisationGLEW(glewInit());

	// Si l'initialisation a échoué :
	if (initialisationGLEW != GLEW_OK)
	{

		// On affiche l'erreur grâce à la fonction : glewGetErrorString(GLenum code)
		std::cout << "Erreur d'initialisation de GLEW : " << glewGetErrorString(initialisationGLEW) << std::endl;

		glfwTerminate();
		exit(EXIT_FAILURE);

	}

    glfwSetWindowShouldClose(window, false);

    //===========INITIALIZATION===========

    //====Shader====
    string const vertexShaderFile = "shader.vert";
    string const fragmentShaderFile = "shader.frag";

    GLuint programID = 0;

    GLuint vertexID = 0;
    GLuint fragmentID = 0;

	if (glIsShader(vertexID) == GL_TRUE)
		glDeleteShader(vertexID);

	if (glIsShader(fragmentID) == GL_TRUE)
		glDeleteShader(fragmentID);

	if (glIsProgram(programID) == GL_TRUE)
		glDeleteProgram(programID);

	if (!compilerShader(vertexID, GL_VERTEX_SHADER, vertexShaderFile))
		exit(EXIT_FAILURE);

	if (!compilerShader(fragmentID, GL_FRAGMENT_SHADER, fragmentShaderFile))
		exit(EXIT_FAILURE);

	programID = glCreateProgram();

	glAttachShader(programID, vertexID);
	glAttachShader(programID, fragmentID);

	glBindAttribLocation(programID, 0, "in_Vertex");

	glLinkProgram(programID);

	GLint erreurLink(0);
	glGetProgramiv(programID, GL_LINK_STATUS, &erreurLink);

	if (erreurLink != GL_TRUE)
	{

		GLint tailleErreur(0);
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &tailleErreur);

		char *erreur = new char[tailleErreur + 1];

		glGetShaderInfoLog(programID, tailleErreur, &tailleErreur, erreur);
		erreur[tailleErreur] = '\0';

		cout << erreur << endl;

		delete[] erreur;
		glDeleteProgram(programID);

		exit(EXIT_FAILURE);
	}

    //====Objects====

	int sizeVerticesBytes = sizeof(float) * 12;

	float vertices[] = {-1.0, -1.0,   -1.0, 1.0,    1.0, -1.0,
                        -1.0, 1.0,    1.0, 1.0,     1.0, -1.0};

	GLuint idVAO;
	GLuint idVBO;

	if (glIsBuffer(idVBO) == GL_TRUE)
		glDeleteBuffers(1, &idVBO);

	glGenBuffers(1, &idVBO);

	glBindBuffer(GL_ARRAY_BUFFER, idVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeVerticesBytes, 0, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeVerticesBytes, vertices);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (glIsVertexArray(idVAO) == GL_TRUE)
		glDeleteVertexArrays(1, &idVAO);

	glGenVertexArrays(1, &idVAO);

	glBindVertexArray(idVAO);

		glBindBuffer(GL_ARRAY_BUFFER, idVBO);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
			glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

    //====Texture====


    GLuint gradientMap = SOIL_load_OGL_texture
    (
        "pal.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    //====================================

    vec2 center = vec2(0);
    float scale = 1.0;
    int iter = 10;

    Controller* controller = new Controller(); // a Controller to bind the ESCAPE key to the Window

    controller->bind(GLFW_KEY_ESCAPE, [&window]() {

        glfwSetWindowShouldClose(window, true);

    });

    controller->bind(GLFW_KEY_D, [&center, &scale]() {

        center.x -= 0.01 * scale;

    });

    controller->bind(GLFW_KEY_A, [&center, &scale]() {

        center.x += 0.01 * scale;

    });

    controller->bind(GLFW_KEY_W, [&center, &scale]() {

        center.y -= 0.01 * scale;

    });

    controller->bind(GLFW_KEY_S, [&center, &scale]() {

        center.y += 0.01 * scale;

    });

    controller->bind(GLFW_KEY_J, [&scale]() {

        scale *= 1.01;

    });

    controller->bind(GLFW_KEY_U, [&scale]() {

        scale *= 0.99;

    });

    controller->bind(GLFW_KEY_I, [&iter]() {

        iter++;

    });

    controller->bind(GLFW_KEY_K, [&iter]() {

        iter--;

    });

    int framerate = 60/1000;

    while(!glfwWindowShouldClose(window))
    {

	    int startTime = int(glfwGetTime() * 1000);

        glfwMakeContextCurrent(window);

        controller->check(window);

        glViewport(0, 0, width, height);

        glClearColor(0.2, 0.2, 0.2, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glEnable(GL_DEPTH_TEST);
	    glDisable(GL_CULL_FACE);

        glUseProgram(programID);

            glBindVertexArray(idVAO);

	            envoyer1I("gradient", programID, 0);

                envoyer1I("iter", programID, iter);
                envoyerFloat("scale", programID, scale);
	            envoyerVec2("center", programID, center);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, gradientMap);

                glDrawArrays(GL_TRIANGLES, 0, 6);

            glBindVertexArray(0);

	    glUseProgram(0);

        glfwSwapBuffers(window);

        glfwPollEvents();

        int endTime = int(glfwGetTime() * 1000);
        int elapsedTime = endTime - startTime;

        if(elapsedTime < framerate)
            this_thread::sleep_for(std::chrono::milliseconds(framerate - elapsedTime));

    };

    return EXIT_SUCCESS;
}
