/*
 * RenderingEngine.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: John Hall
 */

#include "RenderingEngine.h"

#include <iostream>

//cpp file purposely included here because it just contains some global functions
#include "ShaderTools.h"

RenderingEngine::RenderingEngine() {
	shaderProgram = ShaderTools::InitializeShaders();
	if (shaderProgram == 0) {
		std::cout << "Program could not initialize shaders, TERMINATING" << std::endl;
		return;
	}

	zoom = 1;
	xVal = 0;
	yVal = 0;
	rotVal = 0;
	gsR = 1;
	gsG = 1;
	gsB = 1;
	vign = 0;
	neg = 0;
	sobelV = 0;
	sobelH = 0;
	sobelU = 0;
	g_3=0;
	g_5=0;
	g_7=0;

	//Passing data from program into our shader
	location = glGetUniformLocation(shaderProgram, "zoom");
	locationX = glGetUniformLocation(shaderProgram, "xVal"); 
	locationY = glGetUniformLocation(shaderProgram, "yVal"); 
	locationRot = glGetUniformLocation(shaderProgram, "rotVal");
	locationR = glGetUniformLocation(shaderProgram, "gsR");
	locationG = glGetUniformLocation(shaderProgram, "gsG");
	locationB = glGetUniformLocation(shaderProgram, "gsB");
	locationV = glGetUniformLocation(shaderProgram, "vign");
	locationNeg = glGetUniformLocation(shaderProgram,"neg");
	locationsV = glGetUniformLocation(shaderProgram, "sobelV");
	locationsH = glGetUniformLocation(shaderProgram, "sobelH");
	locationsU = glGetUniformLocation(shaderProgram, "sobelU"); 
	locationg_3= glGetUniformLocation(shaderProgram, "g_3");
	locationg_5= glGetUniformLocation(shaderProgram, "g_5");
	locationg_7= glGetUniformLocation(shaderProgram, "g_7");
	
}

RenderingEngine::~RenderingEngine() {

}

void RenderingEngine::RenderScene(const std::vector<Geometry>& objects) {
	//Clears the screen to a dark grey background
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(shaderProgram);

	//To pass new data into shader
	glUniform1f(location, zoom); 
	glUniform1f(locationX, xVal);
	glUniform1f(locationY, yVal);
	glUniform1f(locationRot, rotVal);
	glUniform1f(locationR, gsR);
	glUniform1f(locationG, gsG);
	glUniform1f(locationB, gsB);
	glUniform1i(locationV, vign);
	glUniform1i(locationNeg, neg);
	glUniform1i(locationsV, sobelV);
	glUniform1i(locationsH, sobelH);
	glUniform1i(locationsU, sobelU);
	glUniform1i(locationg_3, g_3);
	glUniform1i(locationg_5, g_5);
	glUniform1i(locationg_7, g_7);
	
	for (const Geometry& g : objects) {
		glBindVertexArray(g.vao);
		glDrawArrays(g.drawMode, 0, g.verts.size());

		// reset state to default (no shader or geometry bound)
		glBindVertexArray(0);
	}
	glUseProgram(0);


	// check for an report any OpenGL errors
	CheckGLErrors();
}

void RenderingEngine::assignBuffers(Geometry& geometry) {
	//Generate vao for the object
	//Constant 1 means 1 vao is being generated
	glGenVertexArrays(1, &geometry.vao);
	glBindVertexArray(geometry.vao);

	//Generate vbos for the object
	//Constant 1 means 1 vbo is being generated
	glGenBuffers(1, &geometry.vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBuffer);
	//Parameters in order: Index of vbo in the vao, number of primitives per element, primitive type, etc.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	/*glGenBuffers(1, &geometry.normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.normalBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);*/

	/*glGenBuffers(1, &geometry.colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.colorBuffer);
	//Parameters in order: Index of vbo in the vao, number of primitives per element, primitive type, etc.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);*/

	glGenBuffers(1, &geometry.uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.uvBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);


}

void RenderingEngine::setBufferData(Geometry& geometry) {
	//Send geometry to the GPU
	//Must be called whenever anything is updated about the object
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * geometry.verts.size(), geometry.verts.data(), GL_STATIC_DRAW);

	/*glBindBuffer(GL_ARRAY_BUFFER, geometry.normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * geometry.normals.size(), geometry.normals.data(), GL_STATIC_DRAW);*/

	/*glBindBuffer(GL_ARRAY_BUFFER, geometry.colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * geometry.colors.size(), geometry.colors.data(), GL_STATIC_DRAW);*/

	glBindBuffer(GL_ARRAY_BUFFER, geometry.uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * geometry.uvs.size(), geometry.uvs.data(), GL_STATIC_DRAW);
}

void RenderingEngine::deleteBufferData(Geometry& geometry) {
	glDeleteBuffers(1, &geometry.vertexBuffer);
	glDeleteBuffers(1, &geometry.normalBuffer);
	glDeleteBuffers(1, &geometry.colorBuffer);
	glDeleteBuffers(1, &geometry.uvBuffer);
	glDeleteVertexArrays(1, &geometry.vao);
}

bool RenderingEngine::CheckGLErrors() {
	bool error = false;
	for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError())
	{
		std::cout << "OpenGL ERROR:  ";
		switch (flag) {
		case GL_INVALID_ENUM:
			std::cout << "GL_INVALID_ENUM" << std::endl; break;
		case GL_INVALID_VALUE:
			std::cout << "GL_INVALID_VALUE" << std::endl; break;
		case GL_INVALID_OPERATION:
			std::cout << "GL_INVALID_OPERATION" << std::endl; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl; break;
		case GL_OUT_OF_MEMORY:
			std::cout << "GL_OUT_OF_MEMORY" << std::endl; break;
		default:
			std::cout << "[unknown error code]" << std::endl;
		}
		error = true;
	}
	return error;
}
