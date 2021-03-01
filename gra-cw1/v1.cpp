#include <iostream>
#include <string>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

void draw_plane1();
void draw_plane2();

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods); //closes window when escape key is closed


int main() {

	//intialise GLFW
	if (!glfwInit()) {
		std::cout << "GLFW intialisation failed." << std::endl;
		return -1;
	}


	//Configure OpenGL used in GLFW window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //minimum required OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //what is a minor context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); //using the COMPAT profile (depends on particular version of openGL)

	//create a window
	GLFWwindow* window = glfwCreateWindow(600, 600, "Hello World", NULL, NULL); // Hello world as title. glfwGetPrimaryMonitor() creates full screen window
	if (!window)
	{
		std::cout << "GLFW failed to create window." << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);


	glfwMakeContextCurrent(window);
	glClearColor(0.4f, 0.5f, 0.6f, 1.0f); //this is kept out the render because it only needs to eb called once
	//glClear() clears buffers

	//initalise GLEW as a loading library to access the OpenGL functionality before the render loop
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "GLEW intialisation failed:" << glewGetErrorString(err) << std::endl;
		return -1;
	}
	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//to save computing power, we can enable face culling
	//The back-face culling prevents the program from rending those objects (e.g. triangles) that face away from the camera.
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);


	//create a string
	std::string sequence = "fwfafwf";

	//create the render loop
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glPushMatrix();

		//draw_plane1();
		glRotatef(0.05f, 0, 0.5f, 0);
		draw_plane1();
		//glPopMatrix();

		draw_plane2();

		// restore transformation state
		glfwSwapBuffers(window); // swaps the front buffer (the one your monitor is currently displaying) and the back buffer(the one you draw on) of the specified window, so that the
			//same buffer is not drawn and displayed at the same time, this eliminates flickering. Also known as double buffering.
		glfwPollEvents(); // processes events in the event queue, such as mouse and keyboard input.
	}

	glfwTerminate(); //terminate program

	return 0;
}

void draw_plane1()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); //sets modelview as the current matrix
	//glLoadIdentity();
	glBegin(GL_TRIANGLES);

	// Back face A
	glColor4f(1.0f, 0.0f, 0.0f, 0.5f); // Red !!!!!!!!
	glVertex3f(0, 0.05f, 0);
	glVertex3f(0.05f, 0, 0);
	glVertex3f(-0.05f, 0, 0);

	// Bottom face B
	glColor4f(0.0f, 0.0f, 1.0f, 0.5f); // Blue !!!!!!!!!!!!!!!!!!
	glVertex3f(0, 0, 0.1f);
	glVertex3f(-0.05f, 0, 0);
	glVertex3f(0.05f, 0, 0);

	//Left side face C
	glColor4f(0.0f, 1.0f, 1.0f, 0.5f); // Cyan !!!!!!
	glVertex3f(0, 0.05f, 0);
	glVertex3f(-0.05f, 0, 0);
	glVertex3f(0, 0, 0.1f);

	//Right side face D
	glColor4f(0.0f, 1.0f, 0.0f, 0.5f); // Green !!!!!!!!!!
	glVertex3f(0, 0.05f, 0);
	glVertex3f(0, 0, 0.1f);
	glVertex3f(0.05f, 0, 0);

	glEnd();

}

void draw_plane2()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); //sets modelview as the current matrix
	//glLoadIdentity();
	glBegin(GL_TRIANGLES);

	// Back face A
	glColor4f(1.0f, 0.0f, 0.0f, 0.5f); // Red !!!!!!!!
	glVertex3f(0.5f, 0.05f, 0);
	glVertex3f(0.55f, 0, 0);
	glVertex3f(0.45f, 0, 0);

	// Bottom face B
	glColor4f(0.0f, 0.0f, 1.0f, 0.5f); // Blue !!!!!!!!!!!!!!!!!!
	glVertex3f(0.5f, 0, 0.1f);
	glVertex3f(0.45f, 0, 0);
	glVertex3f(0.55f, 0, 0);

	//Left side face C
	glColor4f(0.0f, 1.0f, 1.0f, 0.5f); // Cyan !!!!!!
	glVertex3f(0.5f, 0.05f, 0);
	glVertex3f(0.45f, 0, 0);
	glVertex3f(0.5f, 0, 0.1f);

	//Right side face D
	glColor4f(0.0f, 1.0f, 0.0f, 0.5f); // Green !!!!!!!!!!
	glVertex3f(0.5f, 0.05f, 0);
	glVertex3f(0.5f, 0, 0.1f);
	glVertex3f(0.55f, 0, 0);

	glEnd();

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) //closes window when escape key is closed
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		std::cout << "Esc key is pressed." << std::endl;
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		glRotatef(5.0f, 0.f, 0.f, 1.f);
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		glRotatef(-5.0f, 0.f, 0.f, 1.f);
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		glLoadIdentity();
	}
}

