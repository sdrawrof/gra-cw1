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

	p1.x1 = 0.f;
	p1.y1 = 0.f;
	p1.z1 = 0.5f;

	p1.x2 = 0.f;
	p1.y2 = 0.1f;
	p1.z2 = 0.f;

	p2.x1 = 0.3f;
	p2.y1 = 0.f;
	p2.z1 = 0.5f;

	p2.x2 = 0.3f;
	p2.y2 = -0.1f;
	p2.z2 = 0.f;

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

	std::cout << "centre point of plane " << pID << std::endl;
	std::cout << "X: " << mAverageX << std::endl;
	std::cout << "Y: " << mAverageY << std::endl;
	std::cout << "Z: " << mAverageZ << std::endl;
	std::cout << std::endl;

	return XYZ(mAverageX, mAverageY, mAverageZ);
}


glm::vec3 MainLoop::Plane::calculateDirection(float x1, float y1, float z1, float x2, float y2, float z2) {

	float x = (x1 - x2);
	float y = (y1 - y2);
	float z = (z1 - z2);

	//get planes forward direction (refactor/put in method later)
	glm::vec3 pDirection = glm::vec3(x,y,z);

	//normalise (for some resason it wouldnt let me do it in one line, also there must be a way to do this without vec3 but whatever)
	pDirection = glm::normalize(pDirection);

	//use pointer to access vector elements
	const float* pSource = (const float*)glm::value_ptr(pDirection);

	std::cout << "direction of plane " << pID << std::endl;
	std::cout << "X: " << pSource[0] << std::endl;
	std::cout << "Y: " << pSource[1] << std::endl;
	std::cout << "Z: " << pSource[2] << std::endl;
	std::cout << std::endl;

	return pDirection;
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

	glm::vec3 direction = p->calculateDirection( p->x1, p->y1, p->z1,  p->x2,  p->y2,  p->z2);
		glTranslatef(p->centrePoint.x, p->centrePoint.y, p->centrePoint.z);
		glRotatef(p->xRotate, 1, 0.f, 0.f);
		glRotatef(p->yRotate, 0.f, 1, 0.f);
		glRotatef(p->zRotate, 0.f, 0.f, 1);
		glTranslatef(-(p->centrePoint.x), -(p->centrePoint.y), -(p->centrePoint.z));
		glTranslatef(direction[0] * p->translate, direction[1] * p->translate, direction[2] * p->translate);

		//in matrix form
		const float* vec = (const float*)glm::value_ptr(direction);

		glm::mat4 centreMatrix = glm::mat4(
			1.f, 0.f, 0.f, vec[0],
			0.f, 1.f, 0.f, vec[1],
			0.f, 0.f, 1.f, vec[2],
			0.f, 0.f, 0.f, 1.f
		);

		//glm::mat4 translated = glm::translate(centreMatrix, glm::vec3(p->centrePoint.x, p->centrePoint.y, p->centrePoint.z));
		glm::mat4 rotationX = glm::rotate(centreMatrix, p->xRotate, glm::vec3(p->x1,0.f,0.f));
		glm::mat4 rotationY = glm::rotate(rotationX, p->yRotate, glm::vec3(0.f, p->y1, 0.f));
		glm::mat4 rotationMatrix = glm::rotate(rotationY, p->zRotate, glm::vec3(0.f, 0.f, p->z1));
		//glm::mat4 reverseTranslated = glm::translate(rotationMatrix, glm::vec3(-(p->centrePoint.x), -(p->centrePoint.y), -(p->centrePoint.z)));
		glm::mat4 translater = glm::translate(rotationMatrix, glm::vec3(vec[0] * p->translate, vec[1] * p->translate, vec[2] * p->translate));

		
		glm::vec4 v1 = translater[0];
		glm::vec4 v2 = translater[1];
		glm::vec4 v3 = translater[2];

		const float* xVec = (const float*)glm::value_ptr(v1);
		const float* yVec = (const float*)glm::value_ptr(v2);
		const float* zVec = (const float*)glm::value_ptr(v3);

		p->x1 = (float) xVec[3];
		p->y1 = (float) yVec[3];
		p->z1 = (float) zVec[3];

		//second point

		const float* vecT = (const float*)glm::value_ptr(direction);

		glm::mat4 centreMatrix1 = glm::mat4(
			1.f, 0.f, 0.f, vecT[0],
			0.f, 1.f, 0.f, vecT[1],
			0.f, 0.f, 1.f, vecT[2],
			0.f, 0.f, 0.f, 1.f
		);

		//glm::mat4 translated = glm::translate(centreMatrix, glm::vec3(p->centrePoint.x, p->centrePoint.y, p->centrePoint.z));
		glm::mat4 rotationX1 = glm::rotate(centreMatrix1, p->xRotate, glm::vec3(p->x2, 0.f, 0.f));
		glm::mat4 rotationY1 = glm::rotate(rotationX1, p->yRotate, glm::vec3(0.f, p->y2, 0.f));
		glm::mat4 rotationMatrix1 = glm::rotate(rotationY1, p->zRotate, glm::vec3(0.f, 0.f, p->z2));
		//glm::mat4 reverseTranslated = glm::translate(rotationMatrix, glm::vec3(-(p->centrePoint.x), -(p->centrePoint.y), -(p->centrePoint.z)));
		glm::mat4 translater1 = glm::translate(rotationMatrix1, glm::vec3(vecT[0] * p->translate, vecT[1] * p->translate, vecT[2] * p->translate));

		glm::vec4 vv1 = translater1[0];
		glm::vec4 vv2 = translater1[1];
		glm::vec4 vv3 = translater1[2];

		const float* xVecT = (const float*)glm::value_ptr(vv1);
		const float* yVecT = (const float*)glm::value_ptr(vv2);
		const float* zVecT = (const float*)glm::value_ptr(vv3);

		p->x2 = (float) xVecT[3];
		p->y2 = (float) yVecT[3];
		p->z2 = (float) zVecT[3];
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
	pID(pIDGenerator++), centrePoint(0, 0, 0), direction(0, 0, 0){

	//centrePoint = calculateCentrePoint();
	//direction = calculateDirection();

};
