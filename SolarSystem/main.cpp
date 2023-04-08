#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <iostream>
#include <stdlib.h>
#include <Model.h>
#include<glm/glm.hpp>
#include<glm/ext.hpp>
#include <GL/glut.h>


using namespace std;

double rotation = 0.0;

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (rotation < 360.0) {
		rotation += 1.0;
	}
	else {
		rotation = 0.0;
	}

	glPushMatrix();
		glRotatef(90.0, 1, 0, 0);
		glColor3f(9.0, 9.0, 9.0);
		glTranslatef(0.0, 0.0, 0.0);
		glRotatef(-rotation, 0, 0, 1);
		glutWireSphere(0.5, 20, 16);
	glPopMatrix();

	glPushMatrix();
		glRotatef(90.0, 1, 0, 0);
		glColor3f(9.0, 9.0, 9.0);
		glTranslatef(0.0, 0.0, 0.0);
		glRotatef(-rotation, 0, 0, 1);
		glTranslatef(2.0, 0, 0.0);
		glutWireSphere(0.2, 10, 8);
	glPopMatrix();

	glutSwapBuffers();

	glutPostRedisplay();
}

void reshape(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	gluPerspective(50.0, width / (GLfloat)height, 3.0, 90.0);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(2.0, 2.0, 5.0, 
		0.0, 0.0, 0.0, 
		0.0, 1.0, 0.0);
}

int main(int argc, char** argv) {
	
	glutInit(&argc, argv);
	//D:\Studia\OpenGL\Projekt
	Model model;
	model.loadFromFile("Œcie¿ka  mercurio.obj");

	cout << model.getVertexCount() << endl;

	return 0;

}
