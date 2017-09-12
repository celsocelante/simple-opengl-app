#include <iostream>
#include <string>
#include <GLUT/glut.h>
#include <math.h>
// #include <GL/glut.h>
#include "tinyxml2/tinyxml2.h"
#include "Circle.h"
using namespace std;

Circle circ = Circle(0.5, 0.5, 0, 0.1);


void display(void) {
    if(circ.getDisplayed()) {
    }

    GLfloat twicePi = 2.0f * 3.14;

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(0.001);
    
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINES);
        glColor4f(1.0, 1, 1, 1.0);

        for(int i = 0; i <= 1000; i++) {
            glVertex3f(circ.getX(), circ.getY(), 0);
            glVertex3f(circ.getX() + (circ.getRadius() * cos(i * twicePi / 1000)), circ.getY() + (circ.getRadius() * sin(i * twicePi / 1000)), 0);
        }
    glEnd();

    glFlush ();
}

void onMouseClick(int button, int state, int x, int y) {
    double ox;
    double oy;

    ox = (x * 1.0)/300;
    oy = -((y * 1.0)/300 - 1.0);

    // printf("%f %f\n", ox, oy);

    circ.setCoord(ox, oy, 0);

    glutPostRedisplay();
}

void onMouseMove(int x, int y) {
    double ox;
    double oy;

    ox = (x * 1.0)/300;
    oy = ((y * 1.0)/300 - 1.0);

    printf("%f %f\n", ox, oy);

    circ.setCoord(circ.getX() + ox, circ.getY() + oy, 0);

    glutPostRedisplay();
}

void init(void) {
    /* selecionar cor de fundo (preto) */
    glClearColor (0.0, 0.0, 0.0, 0.0);
    /* inicializar sistema de viz. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void readConfigFile(const char* fileName) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(fileName);

    string title = doc.FirstChildElement("aplicacao")->FirstChildElement("janela")->FirstChildElement("largura")->GetText();
    cout << title << '\n';
}

int main(int argc, char** argv) {
    readConfigFile("config.xml");
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (300, 300);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("hello");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(onMouseClick);
    // glutMotionFunc(onMouseMove);
    glutMainLoop();
    /* C ANSI requer que main retorne um inteiro */
    return 0;
}