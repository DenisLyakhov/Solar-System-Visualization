#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>
#include <DataLoader.h>
#include <DataParser.h>
#include <Planet.h>
#include<DefaultCelestialBodyData.h>

using namespace std;


const map<string, string> DefaultCelestialBodyData::MERCURY = { {"CENTER","199@399"} };
const map<string, string> DefaultCelestialBodyData::VENUS = { {"CENTER","299@399"} };
const map<string, string> DefaultCelestialBodyData::EARTH = { {"CENTER","399@399"} };
const map<string, string> DefaultCelestialBodyData::MARS = { {"CENTER","499@399"} };
const map<string, string> DefaultCelestialBodyData::JUPITER = { {"CENTER","599@399"} };
const map<string, string> DefaultCelestialBodyData::SATURN = { {"CENTER","699@399"} };
const map<string, string> DefaultCelestialBodyData::URANUS = { {"CENTER","799@399"} };
const map<string, string> DefaultCelestialBodyData::NEPTUNE = { {"CENTER","899@399"} };
const map<string, string> DefaultCelestialBodyData::SUN = { {"CENTER","10@399"} };


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


	Planet body = Planet("mars");
	list<VectorData> vectorList = body.getVectorData();


	for (auto v : vectorList) {
		cout << v.toString() << endl;
	}
	glutInit(&argc, argv);
	
	/*
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Solar System");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	*/
	return 0;

}
