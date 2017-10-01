#include <iostream>
#include <string>
#include <math.h>
#include "tinyxml2/tinyxml2.h"
#include "Circle.h"
#include "Window.h"

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif


using namespace std;

// Initial circle object to be manipulated by the user
Circle circ;

// Initial window object
Window win;

// Mouse motion history
double xHistory[2];
double yHistory[2];

bool initDone = false;


void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);     
    glPushMatrix(); 
    glClearColor (win.getRed(), win.getGreen(), win.getBlue(), 0);

    // Draw a circle right after the first mouse click
    if(circ.getDisplayed() && initDone) {
        circ.draw();
    }
    glFlush();
}

void init(void) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, win.getWidth(), win.getHeight(), 0.0, -1.0, 1.0);
    glFlush();
    initDone = true;
}

void onMouseClick(int button, int state, int x, int y) {
    // Circle released
    if(state == GLUT_UP && circ.getDragState()) {
        circ.setDragState(false);
        circ.setResizeState(false);

        return;
    }

    // Not being resized anymore
    if(state == GLUT_UP && circ.getResizeState()) {
        circ.setResizeState(false);
        circ.setDragState(false);
        return;
    }

    if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
        // If this is the first click...
        if( !circ.getDisplayed() ) {

            circ.setCoord(x, y, 0);
            circ.setDisplayed(true);
        
        // If the circle already exists
        } else {
            // If clicked within the circle
            if( sqrt(pow(x - circ.getX(), 2) + pow(y - circ.getY(), 2)) <= (circ.getRadius()) ) {
                circ.setDragState(true);
                
                // Init coordinates history
                xHistory[0] = x;
                yHistory[0] = y;

                xHistory[1] = x;
                yHistory[1] = y;
            }
        }
        return;
    }


    if(state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON && circ.getDisplayed() &&
            sqrt(pow(x - circ.getX(), 2) + pow(y - circ.getY(), 2)) <= (circ.getRadius()) ) {
                
        circ.setResizeState(true);
        
        // Init coordinates history
        xHistory[0] = x;
        yHistory[0] = y;

        xHistory[1] = x;
        yHistory[1] = y;
    }
}

void onMouseMove(int x, int y) {
    if( circ.getDragState() ) {
        // Update history
        xHistory[0] = xHistory[1];
        yHistory[0] = yHistory[1];

        xHistory[1] = x;
        yHistory[1] = y;

        double deltaX = xHistory[1] - xHistory[0];
        double deltaY = yHistory[1] - yHistory[0];

        circ.setCoord(circ.getX() + (deltaX), circ.getY() + (deltaY), 0);

        glutPostRedisplay();

    } else if(circ.getResizeState()) {
        // Update history
        xHistory[0] = xHistory[1];
        yHistory[0] = yHistory[1];

        xHistory[1] = x;
        yHistory[1] = y;

        // Distance to resize the circle
        double deltaX = circ.getX() - x;
        double deltaY = circ.getY() - y;

        double distance = sqrt(pow(deltaX, 2) + pow(deltaY, 2));

        if(distance > circ.getRadius()) {
            circ.setRadius(circ.getRadius() + distance / 5);
        } else {
            circ.setRadius(circ.getRadius() - distance / 5);
        }

        glutPostRedisplay();
    }
    glutPostRedisplay();
}

void idle(void)
{   
    glutPostRedisplay();
}

void readConfigFile(string fileName) {
    char path[200];
    strcpy(path, fileName.c_str());
    strcat(path, "config.xml");

    tinyxml2::XMLDocument doc;
    doc.LoadFile(path);

    // Window properties
    int largura = stoi(doc.FirstChildElement("aplicacao")->FirstChildElement("janela")->FirstChildElement("largura")->GetText());
    int altura = stoi(doc.FirstChildElement("aplicacao")->FirstChildElement("janela")->FirstChildElement("altura")->GetText());
    string titulo = doc.FirstChildElement("aplicacao")->FirstChildElement("janela")->FirstChildElement("titulo")->GetText();

    double corR = doc.FirstChildElement("aplicacao")->FirstChildElement("janela")->FirstChildElement("fundo")->DoubleAttribute("corR");
    double corG = doc.FirstChildElement("aplicacao")->FirstChildElement("janela")->FirstChildElement("fundo")->DoubleAttribute("corG");
    double corB = doc.FirstChildElement("aplicacao")->FirstChildElement("janela")->FirstChildElement("fundo")->DoubleAttribute("corB");

    win = Window(largura, altura, titulo);
    win.setRGB(corR, corG, corB);

    // Circle properties
    double raio = doc.FirstChildElement("aplicacao")->FirstChildElement("circulo")->DoubleAttribute("raio");
    corR = doc.FirstChildElement("aplicacao")->FirstChildElement("circulo")->DoubleAttribute("corR");
    corG = doc.FirstChildElement("aplicacao")->FirstChildElement("circulo")->DoubleAttribute("corG");
    corB = doc.FirstChildElement("aplicacao")->FirstChildElement("circulo")->DoubleAttribute("corB");

    circ = Circle(0, 0, 0, raio);
    circ.setRGB(corR, corG, corB);
}

int main(int argc, char** argv) {
    if(argc == 2) {
        readConfigFile(argv[1]);
    } else {
        cerr << "Numero invalido de argumentos\n";
        exit(1);
    }

    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize (win.getWidth(), win.getHeight());
    glutInitWindowPosition (100, 100);
    glutCreateWindow (win.getTitle().c_str());

    init();

    glutDisplayFunc(display);
    glutMouseFunc(onMouseClick);
    glutMotionFunc(onMouseMove);
    glutIdleFunc(idle);

    glutMainLoop();

    return 0;
}