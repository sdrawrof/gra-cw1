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

	glm::mat4x3 planematrix1 = p1.matrix;
	glm::mat4x3 planematrix2 = p2.matrix;

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

	std::cout << "centre point of plane" << std::endl;
	std::cout << "X: " << mAverageX << std::endl;
	std::cout << "Y: " << mAverageY << std::endl;
	std::cout << "Z: " << mAverageZ << std::endl;
	std::cout << std::endl;

	return XYZ(mAverageX, mAverageY, mAverageZ);
}


MainLoop::XYZ MainLoop::Plane::calculateDirection() {
	calculateCentrePoint();

	float x = (point.x - centrePoint.x);
	float y = (point.y - centrePoint.y);
	float z = (point.z - centrePoint.z);

	//get planes forward direction (refactor/put in method later)
	glm::vec3 pDirection = glm::vec3(x, y, z);

	//normalise (for some resason it wouldnt let me do it in one line, also there must be a way to do this without vec3 but whatever)
	pDirection = glm::normalize(pDirection);

	//use pointer to access vector elements
	const float* pSource = (const float*)glm::value_ptr(pDirection);

	std::cout << "direction of plane" << std::endl;
	std::cout << "X: " << pSource[0] << std::endl;
	std::cout << "Y: " << pSource[1] << std::endl;
	std::cout << "Z: " << pSource[2] << std::endl;
	std::cout << std::endl;

	return XYZ(pSource[0], pSource[1], pSource[2]);
}

void MainLoop::Plane::updatePlane(glm::vec3 pointV, glm::vec3 leftV, glm::vec3 rightV, glm::vec3 topV) {
	//use pointer to access vector elements
	const float* pPoint = (const float*)glm::value_ptr(pointV);
	const float* pLeft = (const float*)glm::value_ptr(leftV);
	const float* pRight = (const float*)glm::value_ptr(rightV);
	const float* pTop = (const float*)glm::value_ptr(topV);

	point.x = pPoint[0];
	point.y = pPoint[1];
	point.z = pPoint[2];

	bLeft.x = pLeft[0];
	bLeft.y = pLeft[1];
	bLeft.z = pLeft[2];

	bRight.x = pRight[0];
	bRight.y = pRight[1];
	bRight.z = pRight[2];

	bTop.x = pTop[0];
	bTop.y = pTop[1];
	bTop.z = pTop[2];
}

glm::mat4x3 MainLoop::Plane::updatePlaneMatrix() {
	glm::mat4x3 amatrix = glm::mat4x3(
		glm::vec3(point.x, point.y, point.z),
		glm::vec3(bLeft.x, bLeft.y, bLeft.z),
		glm::vec3(bRight.x, bRight.y, bRight.z),
		glm::vec3(bTop.x, bTop.y, bTop.z));

	return amatrix;
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

	//do these change? 
	//std::cout << "Plane coordinates :" << std::endl;
	//std::cout << "top : " << p.bTop.x << ", " << p.bTop.y << ", " << p.bTop.z << std::endl;
	//std::cout << "left : " << p.bLeft.x << ", " << p.bLeft.y << ", " << p.bLeft.z << std::endl;
	//std::cout << "right : " << p.bRight.x << ", " << p.bRight.y << ", " << p.bRight.z << std::endl;
	//std::cout << "tip : " << p.point.x << ", " << p.point.y << ", " << p.point.z << std::endl;
	//std::cout << "centre point : " << p.centrePoint.x << ", " << p.centrePoint.y << ", " << p.centrePoint.z << std::endl;
	//std::cout << std::endl;
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
		//p->calculateDirection();
		//also need to adjust the rotation by angle form x axis the plane is on here
	glTranslatef(p->centrePoint.x, p->centrePoint.y, p->centrePoint.z);
	glRotatef(p->xRotate, 1, 0.f, 0.f);
	glRotatef(p->yRotate, 0.f, 1, 0.f);
	glRotatef(p->zRotate, 0.f, 0.f, 1);
	glTranslatef(-(p->centrePoint.x), -(p->centrePoint.y), -(p->centrePoint.z));
	glTranslatef((p->direction.x) * p->translate, (p->direction.y) * p->translate, (p->direction.z) * p->translate);

	//in matrix form
	glm::mat4 centreMatrix = glm::mat4(
		1.f, 0.f, 0.f, p->centrePoint.x,
		0.f, 1.f, 0.f, p->centrePoint.y,
		0.f, 0.f, 1.f, p->centrePoint.z,
		0.f, 0.f, 0.f, 1.f
	);

	glm::mat4 translated = glm::translate(centreMatrix, glm::vec3(p->centrePoint.x, p->centrePoint.y, p->centrePoint.z));
	glm::mat4 rotationX = glm::rotate(translated, p->xRotate, glm::vec3(1, 0.f, 0.f));
	glm::mat4 rotationY = glm::rotate(rotationX, p->yRotate, glm::vec3(0.f, 1, 0.f));
	glm::mat4 rotationMatrix = glm::rotate(rotationY, p->zRotate, glm::vec3(0.f, 0.f, 1));
	glm::mat4 reverseTranslated = glm::translate(rotationMatrix, glm::vec3(-(p->centrePoint.x), -(p->centrePoint.y), -(p->centrePoint.z)));

	const float* m = (const float*)glm::value_ptr(rotationMatrix);
	std::cout << "Rotated point is: " << std::endl;
	for (int i = 0; i < 16; i++) {
		std::cout << m[i] << std::endl;
	}
	std::cout << std::endl;

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
	pID(pIDGenerator++), centrePoint(0, 0, 0), direction(0, 0, 0), matrix{ 0,0,0,0,0,0,0,0,0,0,0,0 } {

	centrePoint = calculateCentrePoint();
	direction = calculateDirection();
	matrix = updatePlaneMatrix();

};