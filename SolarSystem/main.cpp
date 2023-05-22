#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <iostream>
#include <stdlib.h>
#include <direct.h>
#include "model3DS.h"


using namespace std;

double rotation = 0.0;
int stereoTryb = 0;
int currentModel = 0;

double kameraX = 0;
double kameraY = 0;
double kameraZ = 0;

vector<string> currentModelList = {"earth","venus","untitled"};



struct model_w_skladzie {
	char* filename;
	model3DS* model;
	struct model_w_skladzie* wsk;
};
struct model_w_skladzie* sklad_modeli = NULL;

void dodajModel(model3DS* _model, char* file_name)
{
	struct model_w_skladzie* tmp;
	tmp = (struct model_w_skladzie*)malloc(sizeof(struct model_w_skladzie));
	tmp->filename = (char*)malloc(strlen(file_name) + 1);
	strcpy(tmp->filename, file_name);
	tmp->model = _model;
	tmp->wsk = sklad_modeli;
	sklad_modeli = tmp;
}

model3DS* pobierzModel(char* file_name)
{
	struct model_w_skladzie* sklad_tmp = sklad_modeli;
	while (sklad_tmp) {
		if (!_stricmp(sklad_tmp->filename, file_name)) return sklad_tmp->model;
		char file_name_full[_MAX_PATH];
		strcpy(file_name_full, file_name);
		strcat(file_name_full, ".3ds");
		if (!_stricmp(sklad_tmp->filename, file_name_full)) return sklad_tmp->model;

		sklad_tmp = sklad_tmp->wsk;
	}
	return NULL;
}

void rysujModel(char* file_name, int tex_num = -1)
{
	model3DS* model_tmp;
	if (model_tmp = pobierzModel(file_name))
		if (tex_num == -1)
			model_tmp->draw();
		else
			model_tmp->draw(tex_num, false);

}

void aktywujSpecjalneRenderowanieModelu(char* file_name, int spec_id = 0)
{
	model3DS* model_tmp;
	if (model_tmp = pobierzModel(file_name))
		model_tmp->setSpecialTransform(spec_id);
}
//³adujem modele z folderu
void ladujModele()
{
	WIN32_FIND_DATA* fd;
	HANDLE fh;
	model3DS* model_tmp;
	char directory[_MAX_PATH];
	if (_getcwd(directory, _MAX_PATH) == NULL) return;
	strcat(directory, "\\data\\*.3ds");

	fd = (WIN32_FIND_DATA*)malloc(sizeof(WIN32_FIND_DATA));
	fh = FindFirstFile((LPCSTR)directory, fd);
	if (fh != INVALID_HANDLE_VALUE)
		do {
			if (fd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {	// katalogi ignorujemy
				if (FindNextFile(fh, fd)) continue; else break;
			}
			// ladowanie obiektu i dodanie do kontenera
			char filename[_MAX_PATH];
			strcpy(filename, "data\\");
			strcat(filename, fd->cFileName);
			model_tmp = new model3DS(filename, 1, stereoTryb == 2);
			dodajModel(model_tmp, fd->cFileName);
			printf("[3DS] Model '%s' stored\n", fd->cFileName);
		} while (FindNextFile(fh, fd));
}




void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
	GLfloat  ambient[4] = { 0.3,0.3,0.3,1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

	GLfloat  diffuse[4] = { 0.9,0.9,0.9,1 };
	GLfloat  specular[4] = { 0.9,0.9,0.9,1 };
	GLfloat	 position[4] = { 300,300,300,1 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);  // œwiatlo sceny


	glShadeModel(GL_FLAT);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::string planet = currentModelList.at(currentModel);//Changint planet

	char* charPtr = new char[planet.length() + 1]; 
	std::strcpy(charPtr, planet.c_str());

	glPushMatrix();
	glTranslatef(0, 0.0, 0);
	glRotatef(0, 0, 0, 0);
	rysujModel(charPtr);
	glPopMatrix();

	gluLookAt(0, 0, 0,kameraZ, 0, -1, 0, 1, 0);
	GLfloat	 position[4] = { 300,300,300,1 };
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	kameraX = 0;
	kameraY = 0;
	kameraZ = 0;

	glutSwapBuffers();

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {

	switch (key) {
	case '+':
		if (currentModel < 3) currentModel++;
		else currentModel = 0;
		break;
	case '-':
		if (currentModel > 1) currentModel--;
		else currentModel = 2;
		break;
	case 'z':
		kameraZ-=0.1;
		break;
	case 'x':
		kameraZ+=0.1;
		break;
	case 'c':
		kameraY++;
		break;
	case 'p':
		std::cout << kameraX<< endl;;
		break;
	default:
		break;
	}
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	//D:\Studia\OpenGL\Projekt
	Model model;
	model.loadFromFile("Œcie¿ka  mercurio.obj");

	cout << model.getVertexCount() << endl;

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Solar System");
	init();
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glEnable(GL_DEPTH_TEST);
	gluLookAt(0, 0, 1, 0, 0, -1, 0, 1, 0); // kamera
	ladujModele();

	glutMainLoop();
	return 0;

}