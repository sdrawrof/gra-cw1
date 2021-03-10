#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "../MainLoop.h"
#include "MainLoop.h"
#include <cstdlib>


// Default resolutions
int defaultX = 800;
int defaultY = 800;

// Initialises static members
int MainLoop::keyPress = GLFW_KEY_UNKNOWN;
int MainLoop::Plane::pIDGenerator = 0;

/* Handles key presses. */
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		// Saves them, prints to terminal
		MainLoop::keyPress = key;
		std::cout << MainLoop::keyPress << std::endl;

		// CLOSE WINDOW
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			std::cout << "ESC" << std::endl;
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
}

/* Initialises openGL. */
void initOpenGL() {
	if (!glfwInit()) {
		std::cout << "GLFW initialisation failed." << std::endl;
		exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
}

/* Checks the window is constructed correctly. */
void checkWindow(GLFWwindow* window) {
	if (!window) {
		std::cout << "GLFW failed to create window." << std::endl;
		glfwTerminate();
		exit(1);
	}
}

/* Initialise GLEW as a loading library. */
void initialiseGLEW() {
	glewExperimental = GL_TRUE;
	auto err = glewInit();
	if (err != GLEW_OK) {
		std::cout << "GLEW initialisation failed: "
			<< glewGetErrorString(err) << std::endl;
		exit(-1);
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set background colour
	glClearColor(0.4f, 0.5f, 0.6f, 1.0f);
}

/* Set resolution at command line. */
void setRes(int argc, char **argv) {
	if (argc == 2) {
		std::cout << "Resolution set at command line." 
			<< argv[1] << argv[2] << std::endl;
		defaultX = atoi(argv[1]);
		defaultY = atoi(argv[2]);
	}
}

/* program entry point, duh. */
int main(int argc, char **argv) {
	initOpenGL();
	setRes(argc, argv);
	auto window = glfwCreateWindow(defaultX, defaultY, "Hello World", NULL, NULL);
	checkWindow(window);
	glfwSetKeyCallback(window, keyCallback);
	glfwMakeContextCurrent(window);
	initialiseGLEW();

	MainLoop::mainLoop(window); // Main loop entered

	glfwTerminate();
	return 0;
}
