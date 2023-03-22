#include <GL/glut.h>

#include <iostream>
#include <stdlib.h>
#include <DataLoader.h>
#include <DataParser.h>

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

	// obj_name=Mars&location=500@399&start_time=now&stop_time=+1d&step_size=1h

	DataLoader loader = DataLoader();
	loader.setFormat("format=text");
	loader.setCommand("COMMAND='499'");
	loader.setObjData("OBJ_DATA='YES'");
	loader.setCenter("CENTER='500@399'");
	loader.setStartTime("START_TIME='2006-01-01'");
	loader.setStopTime("STOP_TIME='2006-01-20'");
	loader.setStepSize("STEP_SIZE='1%20d'");
	loader.setQuantities("QUANTITIES='1,9,20,23,24,29'");
	loader.setMakeEphem("MAKE_EPHEM='YES'");
	loader.setEphemType("EPHEM_TYPE='VECTOR'");

	cout << loader.buildUrl() << endl;
	string data = loader.getData().text;
	cout << DataParser::parseData(data) << endl;
	list<VectorData> vectorList = DataParser::parseDataVector(data);
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