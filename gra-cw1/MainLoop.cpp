#include "MainLoop.h"
#include "glm/ext.hpp"
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "glm/gtx/string_cast.hpp"

/* The program main loop. */
void MainLoop::mainLoop(GLFWwindow* const window) {
	// Creates 2 new planes
	Plane p1 = Plane(XYZ(0.f, 0.f, 0.5f),
		XYZ(0.1f, -0.1f, 0.f),
		XYZ(-0.1f, -0.1f, 0.f),
		XYZ(0.f, 0.1f, 0.f));

	Plane p2 = Plane(XYZ(0.3f, 0.f, 0.5f),
		XYZ(0.4f, -0.1f, 0.f),
		XYZ(0.2f, -0.1f, 0.f),
		XYZ(0.3f, 0.1f, 0.f));



	// The main loop
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW); //sets modelview as the current matrix mode

		// Plane 1
		glPushMatrix();			    // saves state
		handleTransformations(&p1); // handles transforms
		drawPlane(p1);			    // Draws plane
		glPopMatrix();			    // returns state

		// Plane 2
		glPushMatrix();			    // saves state
		handleTransformations(&p2); // handles transforms
		drawPlane(p2);			    // Draws plane
		glPopMatrix();			    // returns state

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

/* Calculates the centre point of a plane. */
MainLoop::XYZ MainLoop::Plane::calculateCentrePoint() {
	float bAverageX = (bLeft.x + bRight.x) / 2.f;
	float bAverageY = (bLeft.y + bRight.y) / 2.f;
	float bAverageZ = (bLeft.z + bRight.z) / 2.f;

	float mAverageX = (bAverageX + point.x) / 2.f;
	float mAverageY = (bAverageY + point.y) / 2.f;
	float mAverageZ = (bAverageZ + point.z) / 2.f;

	return XYZ(mAverageX, mAverageY, mAverageZ);
}

MainLoop::XYZ MainLoop::Plane::calculateDirection() {
	calculateCentrePoint();

	float x = (point.x - centrePoint.x);
	float y = (point.y - centrePoint.y);
	float z = (point.z - centrePoint.z);

	//get planes forward direction (refactor/put in method later)
	glm::vec3 pDirection = glm::vec3(x,y,z);

	//normalise (for some resason it wouldnt let me do it in one line, also there must be a way to do this without vec3 but whatever)
	pDirection = glm::normalize(pDirection);

	//put values in an array since it wont let me diretly access vector elements
	float directionArray[3] = { 0.0 };
	const float* pSource = (const float*)glm::value_ptr(pDirection);

	std::cout << "direction of plane" << std::endl;
	std::cout << "X: " << pSource[0] << std::endl;
	std::cout << "Y: " << pSource[1] << std::endl;
	std::cout << "Z: " << pSource[2] << std::endl;

	return XYZ(pSource[0], pSource[1], pSource[2]);
}


/* Draws a plane (just a pyramid shape). */
void MainLoop::drawPlane(Plane p) {
	glBegin(GL_TRIANGLES);
	glColor4f(1.0f, 0.0f, 0.0f, 0.5f); // Red
	drawTriangle(p.bTop, p.bLeft, p.bRight);
	glColor4f(0.0f, 0.0f, 1.0f, 0.5f); // Blue
	drawTriangle(p.point, p.bRight, p.bLeft);
	glColor4f(0.0f, 1.0f, 1.0f, 0.5f); // Cyan
	drawTriangle(p.bTop, p.bRight, p.point);
	glColor4f(0.0f, 1.0f, 0.0f, 0.5f); // Green
	drawTriangle(p.bTop, p.point, p.bLeft);
	glEnd();
}

/* Handles transforming planes (rotating, moving) based on key presses.
  Plane is passed in by reference - operations on member variables are
  side effecty.                                                      */
void MainLoop::handleTransformations(Plane* p) {

	// Clockwise around Y
	if ((keyPress == GLFW_KEY_Q && p->pID == 0) ||
		(keyPress == GLFW_KEY_U && p->pID == 1)) {
		MainLoop::keyPress = GLFW_KEY_UNKNOWN;
		
		p->yRotate -= 5.f;
	}

	// Counter-Clockwise around Y
	else if ((keyPress == GLFW_KEY_E && p->pID == 0) ||
		(keyPress == GLFW_KEY_O && p->pID == 1)) {
		MainLoop::keyPress = GLFW_KEY_UNKNOWN;
		
		p->yRotate += 5.f;
	}

	// Clockwise around X
	else if ((keyPress == GLFW_KEY_W && p->pID == 0) ||
		(keyPress == GLFW_KEY_I && p->pID == 1)) {
		MainLoop::keyPress = GLFW_KEY_UNKNOWN;
		
		p->xRotate -= 5.f;
	}

	// counter-Clockwise around X
	else if ((keyPress == GLFW_KEY_S && p->pID == 0) ||
		(keyPress == GLFW_KEY_K && p->pID == 1)) {
		MainLoop::keyPress = GLFW_KEY_UNKNOWN;
		
		p->xRotate += 5.f;
	}

	// Clockwise around Z
	else if ((keyPress == GLFW_KEY_A && p->pID == 0) ||
		(keyPress == GLFW_KEY_J && p->pID == 1)) {
		MainLoop::keyPress = GLFW_KEY_UNKNOWN;
		
		p->zRotate -= 5.f;
	}

	// Counter-Clockwise around Z
	else if ((keyPress == GLFW_KEY_D && p->pID == 0) ||
		(keyPress == GLFW_KEY_L && p->pID == 1)) {
		MainLoop::keyPress = GLFW_KEY_UNKNOWN;
		
		p->zRotate += 5.f;
	}

	// Moves planes forward
	else if ((keyPress == GLFW_KEY_F && p->pID == 0) ||
		(keyPress == GLFW_KEY_H && p->pID == 1)) {
		MainLoop::keyPress = GLFW_KEY_UNKNOWN;
		
		p->translate += 0.1f;
	}

	// Handles key reset events
	else if (keyPress == GLFW_KEY_SPACE) {
		p->xRotate = 0.f;
		p->yRotate = 0.f;
		p->zRotate = 0.f;
		p->translate = 0.f;
	}


	//apply movement: (actual methods are in reverse to this)
	//1. translate plane onto axis for translating
	//2. rotate/translate etc plane
	//3. translate back to original position by reversing translation
	if (p->xRotate != 0)
	{
		p->calculateDirection();
		//also need to adjest the rotation by angle form x axis the plane is on
		glTranslatef(p->centrePoint.x, p->centrePoint.y, p->centrePoint.z);
		glRotatef(p->xRotate, 1, 0.f, 0.f);
		glTranslatef(-(p->centrePoint.x), -(p->centrePoint.y), -(p->centrePoint.z));
	}
	
	
	//glRotatef(p->xRotate, p->centrePoint.x, 0.f, 0.f);
	if (p->yRotate != 0)
	{
		p->calculateDirection();
		glTranslatef(p->centrePoint.x, p->centrePoint.y, p->centrePoint.z);
		glRotatef(p->yRotate, 0.f, 1, 0.f);
		glTranslatef(-(p->centrePoint.x), -(p->centrePoint.y), -(p->centrePoint.z));
	}
	
	if (p->zRotate != 0)
	{
		p->calculateDirection();
		glTranslatef(p->centrePoint.x, p->centrePoint.y, p->centrePoint.z);
		glRotatef(p->zRotate, 0.f, 0.f, 1);
		glTranslatef(-(p->centrePoint.x), -(p->centrePoint.y), -(p->centrePoint.z));
	}
	

	if (p->translate != 0) {
		p->calculateDirection();
		glTranslatef(p->direction.x /10, p->direction.y/10, p->direction.z/10);
	}
		

	// Re-calculates the centre point of the plane
	// Note this currently does nothing, as the transforms
	// do not alter the bLeft, bTop, bRight or point 
	// member variables.
	// To do this a custom transform may need to be implemented?
	p->calculateCentrePoint();
}

/* Draws a triangle. */
void MainLoop::drawTriangle(XYZ p1, XYZ p2, XYZ p3) {
	glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p2.x, p2.y, p2.z);
	glVertex3f(p3.x, p3.y, p3.z);
}

// Constructors for nested classes

/* Constructor for XYZ. */
MainLoop::XYZ::XYZ(const float x, const float y, const float z)
	: x(x), y(y), z(z) {
}

/* Constructor for a plane. */
MainLoop::Plane::Plane(XYZ point, XYZ bLeft,
	XYZ bRight, XYZ bTop) :
	point(point), bLeft(bLeft), bRight(bRight), bTop(bTop),
	pID(pIDGenerator++), centrePoint(0,0,0), direction(0,0,0) {

	centrePoint = calculateCentrePoint();
	direction = calculateDirection();
};
