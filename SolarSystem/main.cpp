#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION

//#include <glad/glad.h>
#include "glut.h"

#include <GL/glut.h>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <list>
#include <math.h>

#include <DataLoader.h>
#include <DataParser.h>
#include <Planet.h>
#include <DefaultCelestialBodyData.h>
#include <direct.h>
#include "model3DS.h"

using namespace std;

const map<string, string> DefaultCelestialBodyData::MERCURY  ={{"COMMAND","199"}};
const map<string, string> DefaultCelestialBodyData::VENUS	 ={{"COMMAND","299"}};
const map<string, string> DefaultCelestialBodyData::EARTH	 ={{"COMMAND","399"}};
const map<string, string> DefaultCelestialBodyData::MARS	 ={{"COMMAND","499"}};
const map<string, string> DefaultCelestialBodyData::JUPITER  ={{"COMMAND","599"}};
const map<string, string> DefaultCelestialBodyData::SATURN   ={{"COMMAND","699"}};
const map<string, string> DefaultCelestialBodyData::URANUS	 ={{"COMMAND","799"}};
const map<string, string> DefaultCelestialBodyData::NEPTUNE  ={{"COMMAND","899"}};
const map<string, string> DefaultCelestialBodyData::SUN		 ={{"COMMAND","10"}};

static DateTime dateTime;

list<Planet> planets;

// View variables

double rotation = 0.0;
int stereoTryb = 0;
int currentModel = 0;

double kameraX = 0;
double kameraY = 0;
double kameraZ = 0;

vector<string> currentModelList = { "mercurio","venus","earth","mars", "jupiter", "saturn", "uranus", "neptune"};

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
//�adujem modele z folderu
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
	dateTime = DateTime();

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
	glEnable(GL_LIGHT0);  // �wiatlo sceny


	glShadeModel(GL_FLAT);
}

float distanceFromCenter = 500.0f;

const float maxDistanceFromCenter = 490.0f;

float lastMousePosX = 0.0;
float lastMousePosY = 0.0;

float lastMouseZoomPos = 0.0f;

float angleX = 0.0f;
float angleY = 0.0f;

// Object variables

double sunSize = 0.5;
double planetSize = 0.5;
double scale = 20000000.0;

// Scene modes
bool dateChangingMode = false;
bool dateValueChanged = false;
bool zoomInMode = false;

void reshape(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	gluPerspective(50.0, width / (GLfloat)height, 3.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y) {

	switch (key) {
	case '+':
		if (currentModel < 7) currentModel++;
		else currentModel = 0;
		break;
	case '-':
		if (currentModel >= 1) currentModel--;
		else currentModel = 7;
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

void enableLighting() {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	GLfloat light1_position[] = { 0.0, 0.0, 10.0, 0.0 };
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
}

void renderGUI() {

	GLfloat x = 10.0f;
	GLfloat y = 10.0f;

	time_t now = time(0);
	char dt[30];
	ctime_s(dt, sizeof dt, &now);
	std::string date_time(dt);

	string guiText = dateTime.toString();

	if (dateChangingMode) {
		guiText += ": Retrieving data...";
	}
	else if (dateValueChanged) {
		guiText += ": Press Enter to apply";
	}

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0f, glutGet(GLUT_WINDOW_WIDTH), 0.0f, glutGet(GLUT_WINDOW_HEIGHT));
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(1.0f, 1.0f, 1.0f);

	glRasterPos2f(x, y);
	for (int i = 0; i < guiText.length(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, guiText[i]);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void renderTextNearObj(float objX, float objY, float objZ, void *font, string string) {
	int viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];

	// Text offset relative to object
	float offsetX = 10.0f;
	float offsetY = 10.0f;

	GLdouble textObjX = objX;
	GLdouble textObjY = objY;
	GLdouble textObjZ = objZ;

	GLdouble winX, winY, winZ;

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	// Convert world coordinates to screen coordinates
	gluProject(textObjX, textObjY, textObjZ,
		modelview, projection, viewport,
		&winX, &winY, &winZ);

	winX += offsetX;
	winY += offsetY;

	// Convert modified screen coordinates back to world coordinates
	gluUnProject(winX, winY, winZ,
		modelview, projection, viewport,
		&textObjX, &textObjY, &textObjZ);

	GLfloat textColor[] = { 1.0f, 1.0f, 1.0f };
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat mat_shininess[] = { 1000.0f };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, textColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	glRasterPos3f(textObjX, textObjY, textObjZ);

	int i = 0;
	while (string[i] != '\0') {
		glutBitmapCharacter(font, string[i]);
		i++;
	}

}

void renderPlanetName(float x, float y, float z, string name) {
	if (name == "mercury" || name == "venus" || name == "earth" || name == "mars") {
		if (distanceFromCenter < 100.0) {
			renderTextNearObj(x, y, z, GLUT_BITMAP_HELVETICA_10, name);
		}
	} else {
		renderTextNearObj(x, y, z, GLUT_BITMAP_HELVETICA_10, name);
	}
}

void renderPlanetOrbit(float x, float y, float z) {
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glColor3f(1.0, 1.0, 1.0);

	double radius = sqrt(x*x + y*y);

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i++)
	{
		float orbit_x = radius * cos(i * M_PI / 180.0f);
		float orbit_y = radius * sin(i * M_PI / 180.0f);
		glVertex3f(orbit_x, orbit_y, 0.0f);
	}
	glEnd();
	glPopMatrix();
}

void renderPlanet(Planet &planet) {
	array<double, 3> coordinates = planet.getFirstCoordinates();
	double x = coordinates[0] / scale;
	double y = coordinates[1] / scale;
	double z = coordinates[2] / scale;

	renderPlanetName(x, y, z, planet.getName());

	enableLighting();

	renderPlanetOrbit(x, y, 0.0f);

	GLfloat red[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);

	glPushMatrix();
	glColor3f(9.0, 9.0, 9.0);
	glTranslatef(x, y, 0.0);
	glutSolidSphere(planet.getSize(), 10, 8);
	glPopMatrix();

	glDisable(GL_LIGHTING);
}

void renderMainScene(void) {

	// Second scene

	// std::string planet = currentModelList.at(currentModel);//Changint planet

	// char* charPtr = new char[planet.length() + 1]; 
	// std::strcpy(charPtr, planet.c_str());

	// glPushMatrix();
	// glTranslatef(0, 0.0, 0);
	// glRotatef(0, 0, 0, 0);
	// rysujModel(charPtr);
	// glPopMatrix();

	// gluLookAt(0, 0, 0, kameraZ, 0, -1, 0, 1, 0);
	// GLfloat	 position[4] = { 300,300,300,1 };
	// glLightfv(GL_LIGHT0, GL_POSITION, position);

	// kameraX = 0;
	// kameraY = 0;
	// kameraZ = 0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	gluLookAt(0.0f, 0.0f, distanceFromCenter,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 1.0f);
	glRotatef(angleX, 1.0f, 0.0f, 0.0f);
	glRotatef(angleY, 0.0f, 0.0f, 1.0f);

	renderGUI();

	for (Planet &p : planets) {
		renderPlanet(p);
	}

	glutSwapBuffers();
	glutPostRedisplay();

	if (dateChangingMode) {
		dateChangingMode = false;
		for (Planet &p : planets) {
			p.retrieveVectorData();
		}
	}
}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
			zoomInMode = true;
		} else {
			zoomInMode = false;
		}
	} else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		lastMousePosX = x;
		lastMousePosY = y;
	}
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 'z' || key == 'x' || key == 'a' || key == 's' || key == 'q' || key == 'w') {
		dateValueChanged = true;
	}

	switch (key) {
	case 'z':
		dateTime.increaseDay();
		break;
	case 'x':
		dateTime.decreaseDay();
		break;
	case 'a':
		dateTime.increaseMonth();
		break;
	case 's':
		dateTime.decreaseMonth();
		break;
	case 'q':
		dateTime.increaseYear();
		break;
	case 'w':
		dateTime.decreaseYear();
		break;
	default:
		break;
	}

	if (key == (char)13) {
		dateChangingMode = true;
		dateValueChanged = false;
	}
}

void handleZoomIn(int mousePosY) {
	if (mousePosY > lastMouseZoomPos && distanceFromCenter < maxDistanceFromCenter) {
		distanceFromCenter += 10.0f;
	}
	else if (mousePosY < lastMouseZoomPos && distanceFromCenter > 10.0f) {
		distanceFromCenter -= 10.0f;
	}
	lastMouseZoomPos = mousePosY;
}

void handleMouseMovement(int x, int y) {
	float dx = (float)(x - lastMousePosX);
	float dy = (float)(y - lastMousePosY);
	angleX += dy / 5.0f;
	angleY += dx / 5.0f;

	if (angleX > 360) {
		angleX -= 360;
	}
	else if (angleX < -360) {
		angleX += 360;
	}
	if (angleY > 360) {
		angleY -= 360;
	}
	else if (angleY < -360) {
		angleY += 360;
	}

	lastMousePosX = x;
	lastMousePosY = y;
}

void motion(int x, int y) {
	if (zoomInMode) {
		handleZoomIn(y);
		return;
	}

	handleMouseMovement(x, y);
}

void displayWindow() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_RGBA);
	glutInitWindowSize(1600, 900);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Solar System");
	init();

	glutMotionFunc(motion);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(renderMainScene);
	glutReshapeFunc(reshape);
	// Second scene

	// glutKeyboardFunc(keyboard);
	// glutDisplayFunc(display);
	
	glEnable(GL_DEPTH_TEST);
	gluLookAt(0, 0, 1, 0, 0, -1, 0, 1, 0); // kamera
	ladujModele();

	glutMainLoop();
}

int main(int argc, char** argv) {

	/*array<Planet, 9> planets = { Planet("mercury"), Planet("venus"), Planet("earth"), Planet("mars"), Planet("jupiter"), Planet("saturn"), Planet("uranus"), Planet("neptune"), Planet("sun") };


	for (Planet planet : planets) {
		VectorData vectorData = planet.getVectorData().front();
		array<double, 3> coords = vectorData.getCoordinatesNumerical();

		cout << planet.name << endl;

		cout << coords[0] << endl;
		cout << coords[1] << endl;
		cout << coords[2] << endl;

		cout << "------------" << endl;
	}*/

	//planets = { Planet("sun", 0.5),  Planet("mercury", 0.1), Planet("jupiter", 0.5), Planet("saturn", 0.5), Planet("uranus", 0.5), Planet("neptune", 0.5) };
	planets = { Planet("sun", 0.5), Planet("mercury", 0.05), Planet("venus", 0.05), Planet("earth", 0.05), Planet("mars", 0.05), Planet("jupiter", 0.5), Planet("saturn", 0.5), Planet("uranus", 0.5), Planet("neptune", 0.5) };
	//planets = { Planet("sun", 0.5) };

	glutInit(&argc, argv);
	
	displayWindow();
	
	return 0;

}
