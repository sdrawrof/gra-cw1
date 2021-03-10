#include "MainLoop.h"
#include "glm/ext.hpp"
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "glm/gtx/string_cast.hpp"
#include <string>

#define GLM_ENABLE_EXPERIMENTAL

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

	//calculate centre points
	p1.centrePoint = p1.calculateCentrePoint(); 
	p1.centrePoint = p1.calculateCentrePoint();

	//get planes forward direction (refactor/put in method later)
	glm::vec3 p1Direction = glm::vec3(p1.bTop.x - p1.centrePoint.x, p1.bTop.y - p1.centrePoint.y, p1.bTop.z - p1.centrePoint.z);
	glm::vec3 p2Direction = glm::vec3(p2.bTop.x - p2.centrePoint.x, p2.bTop.y - p2.centrePoint.y, p2.bTop.z - p2.centrePoint.z);
	 //normalise (for some resason it wouldnt let me do it in one line)
	p1Direction = glm::normalize(p1Direction);
	p2Direction = glm::normalize(p2Direction);

	p1.direction.x = p1Direction[1];
	p1.direction.y = p1Direction[2];
	p1.direction.z = p1Direction[3];

	//check it worked
	//std::cout << "plane 1 direction: " << glm::to_string(p1Direction) << std::endl;
	//std::cout << "plane 2 direction: " << glm::to_string(p2Direction) << std::endl;
	std::cout << "X:" << p1.direction.x << std::endl;
	std::cout << "Y:" <<  p1.direction.y << std::endl;
	std::cout << "Z:" <<  p1.direction.z << std::endl;
	std::cout << " " << std::endl;

	//Then put the planes coordinates into a matrix for transformations (point, left, right, top)
	/*glm::mat4x3 plane1Matrix = glm::mat4x3(
		glm::vec3(p1.point.x, p1.point.y, p1.point.z),
		glm::vec3(p1.bLeft.x, p1.bLeft.y, p1.bLeft.z),
		glm::vec3(p1.bRight.x, p1.bRight.y, p1.bRight.z),
		glm::vec3(p1.bTop.x, p1.bTop.y, p1.bTop.z));

	glm::mat4x3 plane2Matrix = glm::mat4x3(
		glm::vec3(p2.point.x, p2.point.y, p2.point.z),
		glm::vec3(p2.bLeft.x, p2.bLeft.y, p2.bLeft.z),
		glm::vec3(p2.bRight.x, p2.bRight.y, p2.bRight.z),
		glm::vec3(p2.bTop.x, p2.bTop.y, p2.bTop.z));*/
	

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
	// Clockwise around Z
	if ((keyPress == GLFW_KEY_Q && p->pID == 0) ||
		(keyPress == GLFW_KEY_U && p->pID == 1)) {
		MainLoop::keyPress = GLFW_KEY_UNKNOWN;
		p->yRotate -= 5.f;
	}

	// Counter-Clockwise around Z
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

	// Clockwise around Y
	else if ((keyPress == GLFW_KEY_A && p->pID == 0) ||
		(keyPress == GLFW_KEY_J && p->pID == 1)) {
		MainLoop::keyPress = GLFW_KEY_UNKNOWN;
		p->zRotate -= 5.f;
	}

	// Counter-Clockwise around Y
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

	//these should be replaced with draw function that uses rotation values
	//glRotatef(p->xRotate, p->centrePoint.x, 0.f, 0.f);
	//glRotatef(p->yRotate, 0.f, p->centrePoint.y, 0.f);
	//glRotatef(p->zRotate, 0.f, 0.f, p->centrePoint.z);
	//glTranslatef(0.0f, 0.0f, p->translate);

	//rotates around direction of plane
	//glRotatef(p->xRotate, p1Di);
	//glRotatef(p->yRotate, 0.f, p->centrePoint.y, 0.f);
	//glRotatef(p->zRotate, 0.f, 0.f, p->centrePoint.z);
	//glTranslatef(0.0f, 0.0f, p->translate);

	// Re-calculates the centre point of the plane
	// Note this currently does nothing, as the transforms
	// do not alter the bLeft, bTop, bRight or point 
	// member variables.
	// To do this a custom transform may need to be implemented?
	p->centrePoint = p->calculateCentrePoint(); 
	
	//std::cout << "X:" << p->centrePoint.x << std::endl;
	//std::cout << "Y:" <<  p->centrePoint.y << std::endl;
	//std::cout << "Z:" <<  p->centrePoint.z << std::endl;
	//std::cout << " " << std::endl;
}

/* Draws a triangle. */
void MainLoop::drawTriangle(XYZ p1, XYZ p2, XYZ p3)  {
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
	XYZ bRight, XYZ bTop)
	: point(point), bLeft(bLeft), bRight(bRight), bTop(bTop) ,
	pID(pIDGenerator++), centrePoint(calculateCentrePoint()) {

}
	
 