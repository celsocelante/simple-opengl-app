#include <iostream>
#include <string>
#include <GLUT/glut.h>
#include <math.h>
// #include <GL/glut.h>
#include "tinyxml2/tinyxml2.h"
using namespace std;

// void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius){
// 	int i;
// 	int triangleAmount = 20; //# of triangles used to draw circle
	
// 	//GLfloat radius = 0.8f; //radius
// 	GLfloat twicePi = 2.0f * PI;
	
// 	glBegin(GL_TRIANGLE_FAN);
// 		glVertex2f(x, y); // center of circle
// 		for(i = 0; i <= triangleAmount;i++) { 
// 			glVertex2f(
// 		            x + (radius * cos(i *  twicePi / triangleAmount)), 
// 			    y + (radius * sin(i * twicePi / triangleAmount))
// 			);
// 		}
// 	glEnd();
// }

void display(void) {
    /* Limpar todos os pixels */
    glClear (GL_COLOR_BUFFER_BIT);
    /* Desenhar um polígono branco (retângulo) */
    glColor3f (1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
        glVertex3f(0.25, 0.25, 0.0);
        glVertex3f(0.75, 0.25, 0.0);
        glVertex3f(0.75, 0.75, 0.0);
        glVertex3f(0.25, 0.75, 0.0);
    glEnd();

    /* Não esperar! */
    glFlush ();
}

void init(void) {
    /* selecionar cor de fundo (preto) */
    glClearColor (0.0, 0.0, 0.0, 0.0);
    /* inicializar sistema de viz. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void readConfigFile(string fileName) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(fileName);

    string title = doc.FirstChildElement("aplicacao")->FirstChildElement("janela")->FirstChildElement("largura")->GetText();
    cout << title << '\n';
}

int main(int argc, char** argv) {
    readConfigFile("config.xml");
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (250, 250);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("hello");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    /* C ANSI requer que main retorne um inteiro */
    return 0;
}