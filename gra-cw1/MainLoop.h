#pragma once
#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <cmath>
#include <glm/glm.hpp>



// Handles all necessary mainLoop functions sub classes and data
class MainLoop {
	class Plane;

	// Stores all the information about a point in space
	class XYZ {
	public:
		float x;
		float y;
		float z;
		XYZ(const float x, const float y, const float z);
	};

	// Contains all of the information about a plane
	class Plane {
	public:
		static int pIDGenerator; // Generates unique ID for planes
		const int pID;           // The unique plane ID

		// Coordinates
		XYZ centrePoint;// The centre point of the plane
		XYZ point;  // tip of the plane
		XYZ bLeft;  // back left
		XYZ bRight; // back right
		XYZ bTop;   // back top
		XYZ direction; //plane direction

		float xRotate = 0.f; // Stores the amount rotated in x
		float yRotate = 0.f; // Stores the amount rotated in y
		float zRotate = 0.f; // Stores the amount rotated in z
		float translate = 0.f; // Stores the amount translated forward

		glm::mat4x3 matrix;

		XYZ calculateCentrePoint();
		XYZ calculateDirection();
		void updatePlane(glm::vec3 pointV, glm::vec3 leftV, glm::vec3 rightV, glm::vec3 topV);
		glm::mat4x3 updatePlaneMatrix();

		Plane(const XYZ point, const XYZ bLeft,
			const XYZ bRight, const XYZ bTop);
	};

public:
	static int keyPress;						// Stores unhandled key presses
	static void mainLoop(GLFWwindow* const window); // The main loop function

private:
	static void handleTransformations(Plane* p); // Handles transformations of p
	static void drawPlane(Plane p);				 // Draws plane p

	// Draws triangle of points p1, p2, p3
	static void drawTriangle(const XYZ p1, const XYZ p2, const XYZ p3);

};
