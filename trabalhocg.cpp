#include <iostream>
#include <string>
#include <GLUT/glut.h>
#include <math.h>
// #include <GL/glut.h>
#include "tinyxml2/tinyxml2.h"
#include "Circle.h"
#include "Window.h"
using namespace std;

// Initial circle object to be manipulated by the user
Circle circ;

// Initial window object
Window win;

// Circle position in pixels
double originalX;
double originalY;

// Mouse motion history
double xHistory[2];
double yHistory[2];

bool initDone = false;


void display(void) {
    // Draw a circle right after the first mouse click
    if(circ.getDisplayed() && initDone) {
        circ.draw();
        glFlush();
    }
}

void onMouseClick(int button, int state, int x, int y) {
    double ox;
    double oy;

    // Circle released
    if(state == GLUT_UP && circ.getDragState()) {
        circ.setDragState(false);
        originalX = x;
        originalY = y;
        return;
    }

    // Not being resized anymore
    if(state == GLUT_UP && circ.getResizeState()) {
        circ.setResizeState(false);
        return;
    }

    if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
        // If this is the first click...
        if( !circ.getDisplayed() ) {

            originalX = x;
            originalY = y;

            ox = (x * 1.0) / win.getWidth();
            oy = -((y * 1.0) / win.getWidth() - 1.0);

            circ.setCoord(ox, oy, 0);
            circ.setDisplayed(true);
            glutPostRedisplay();
        
        // If the circle already exists
        } else {
            // If clicked within the circle
            if( pow(x - originalX, 2) + pow(y - originalY, 2) <= pow(circ.getRadius() * win.getWidth(), 2) ) {
                cout << "dentro\n";
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


    if(state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON && circ.getDisplayed() && pow(x - originalX, 2) + pow(y - originalY, 2) <= pow(circ.getRadius() * win.getWidth(), 2)) {
        cout << "dentro (direito)\n";
        cout << "Mouse (direito): " << x << " " << y << "\n";
        circ.setResizeState(true);
        
        // Init coordinates history
        xHistory[0] = x;
        yHistory[0] = y;

        xHistory[1] = x;
        yHistory[1] = y;
    }
}

void onMouseMove(int x, int y) {
    // cout << "Motion: " << x << " " << y << "\n";
    if(circ.getDragState()) {
        // Update history
        xHistory[0] = xHistory[1];
        yHistory[0] = yHistory[1];

        xHistory[1] = x;
        yHistory[1] = y;

        // cout << "Previous: " << xHistory[0] << " " << yHistory[0] << "\n";
        // cout << "Current: " << xHistory[1] << " " << yHistory[1] << "\n";

        double deltaX = xHistory[1] - xHistory[0];
        double deltaY = yHistory[1] - yHistory[0];

        // cout << "deltaX: " << deltaX << " ";
        // cout << "deltaY: " << deltaY << "\n";

        circ.setCoord(circ.getX() + (deltaX / win.getWidth()), circ.getY() + (-deltaY / win.getWidth()), 0);
        glutPostRedisplay();

    } else if(circ.getResizeState()) {
        // Update history
        xHistory[0] = xHistory[1];
        yHistory[0] = yHistory[1];

        xHistory[1] = x;
        yHistory[1] = y;

        // cout << "Previous: " << xHistory[0] << " " << yHistory[0] << "\n";
        // cout << "Current: " << xHistory[1] << " " << yHistory[1] << "\n";

        double deltaX = originalX - x;
        double deltaY = originalY - y;

        double distance = sqrt(pow(deltaX, 2) + pow(deltaY, 2));

        cout << distance<< "\n";
        cout << circ.getRadius()  * win.getWidth() << "\n";


        // cout << "deltaX: " << deltaX << " ";
        // cout << "deltaY: " << deltaY << "\n";
        if(distance > circ.getRadius() * win.getWidth()) {
            circ.setRadius(circ.getRadius() + distance / 10000);
        } else {
            circ.setRadius(circ.getRadius() - distance / 10000);
        }
        
        glutPostRedisplay();
    }
}

void init(void) {
    
    glClearColor (win.getRed(), win.getGreen(), win.getBlue(), 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

    
    initDone = true;
}

void readConfigFile(string fileName) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(fileName.c_str());

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

    circ = Circle(0, 0, 0, 0.1);
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
    glutMainLoop();
    return 0;
}