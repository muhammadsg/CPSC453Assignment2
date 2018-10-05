/*
 * Program.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: John Hall
 */

#include "Program.h"

#include <iostream>
#include <string>

//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RenderingEngine.h"
#include "Scene.h"

int sc;
bool reload;
int state;

RenderingEngine* renderer;

Program::Program() {
	sc=1;
	reload=false;
	setupWindow();
}

Program::~Program() {
	//Must be cleaned up in the destructor because these are allocated to the heap
	delete renderingEngine;
	delete scene;
}

void Program::start() {
	renderingEngine = new RenderingEngine();
	scene = new Scene(renderingEngine);

	renderer = renderingEngine;
	//Main render loop
	while(!glfwWindowShouldClose(window)) {
		scene->displayScene();
		glfwSwapBuffers(window);
		glfwPollEvents();
		if (reload==true){
			scene->Reload(sc,renderingEngine->zoom);
			reload=false;
		}
	}

}

void Program::setupWindow() {
	//Initialize the GLFW windowing system
	if (!glfwInit()) {
		std::cout << "ERROR: GLFW failed to initialize, TERMINATING" << std::endl;
		return;
	}

	//Set the custom error callback function
	//Errors will be printed to the console
	glfwSetErrorCallback(ErrorCallback);

	//Attempt to create a window with an OpenGL 4.1 core profile context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	int width = 512;
	int height = 512;
	window = glfwCreateWindow(width, height, "CPSC 453 OpenGL Boilerplate", 0, 0);
	if (!window) {
		std::cout << "Program failed to create GLFW window, TERMINATING" << std::endl;
		glfwTerminate();
		return;
	}

	//Set the custom function that tracks key presses
	glfwSetKeyCallback(window, KeyCallback);

	//Set the custom function that tracks mouse scroll
	glfwSetScrollCallback(window, scroll_callback);

	//Set the custom function that tracks mouse position
	glfwSetCursorPosCallback(window, cursor_position_callback);

	//Bring the new window to the foreground (not strictly necessary but convenient)
	glfwMakeContextCurrent(window);

	//Intialize GLAD (finds appropriate OpenGL configuration for your system)
	if (!gladLoadGL()) {
		std::cout << "GLAD init failed" << std::endl;
		return;
	}

	//Query and print out information about our OpenGL environment
	QueryGLVersion();
}



void Program::QueryGLVersion() {
	// query opengl version and renderer information
	std::string version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
	std::string glslver = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
	std::string renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));

	std::cout << "OpenGL [ " << version << " ] "
		<< "with GLSL [ " << glslver << " ] "
		<< "on renderer [ " << renderer << " ]" << std::endl;
}

void ErrorCallback(int error, const char* description) {
	std::cout << "GLFW ERROR " << error << ":" << std::endl;
	std::cout << description << std::endl;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//Key codes are often prefixed with GLFW_KEY_ and can be found on the GLFW website
	
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		sc=1;
		reload=true;
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		sc=2;
		reload=true;
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		sc=3;
		reload=true;
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
		sc=4;
		reload=true;
	}
	if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
		sc=5;
		reload=true;
	}
	if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
		sc=6;
		reload=true;
	}
	if (key == GLFW_KEY_7 && action == GLFW_PRESS) {
		sc=7;
		reload=true;
	}
	if (key == GLFW_KEY_LEFT){
		renderer->rotVal += 3.14/12;
	}

	if (key == GLFW_KEY_RIGHT){
		renderer->rotVal -= 3.14/12;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
renderer->zoom += yoffset/2;
//std::cout << renderer->zoom << std::endl;
if(renderer->zoom<1)
	renderer->zoom=1;
reload=true;
}


void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	GLboolean leftButtonPressed;

	if(state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		leftButtonPressed = true;
	}
	else if(state == GLFW_RELEASE)
	{
		leftButtonPressed = false;
	}
	if(leftButtonPressed)
		{
			renderer->xVal = xpos/500;
			renderer->yVal = -ypos/500;
			//std::cout << "x:"<< xpos << " y:" << ypos << std::endl;
		}

}