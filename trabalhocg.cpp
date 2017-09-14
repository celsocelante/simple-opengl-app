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
Circle circ = Circle(0.5, 0.5, 0, 0.1);
// Initial window object
Window win = Window(500, 500, "hello");

double originalX;
double originalY;

// If the mouse click was performed once, so do nothing
bool clicked = false;

// Mouse motion history
double xHistory[2];
double yHistory[2];

double convertX(double x, int windowWidth) {
    return (x * 1.0) / windowWidth;
}

double convertY(double y, int windowHeight) {
    return -((y * 1.0) / windowHeight - 1.0);
}


void display(void) {
    // Draw a circle right after the first mouse click
    if(clicked) {
        circ.draw();
    }

    glFlush ();
}



void onMouseClick(int button, int state, int x, int y) {
    double ox;
    double oy;

    cout << button << "\n";

    // Circle released
    if(state == GLUT_UP && circ.getDragState()) {
        circ.setDragState(false);
        return;
    }

    // Not being resized anymore
    if(state == GLUT_UP && circ.getResizeState()) {
        circ.setResizeState(false);
        return;
    }

    if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
        // If this is the first click...
        if(!clicked) {
            originalX = x;
            originalY = y;

            ox = (x * 1.0)/500;
            oy = -((y * 1.0)/500 - 1.0);

            circ.setCoord(ox, oy, 0);

            glutPostRedisplay();
            clicked = true;
        } else {
            double dx = x - originalX;
            double dy = y - originalY;
            double dist_squared = dx * dx + dy * dy;

            // If clicked inside
            if( (x - originalX)*(x - originalX) + (y - originalY)*(y - originalY) <= 50*50 ) {
                cout << "dentro\n";
                cout << "Mouse: " << x << " " << y << "\n";
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


    if(state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON && clicked) {
        double dx = x - originalX;
        double dy = y - originalY;
        double dist_squared = dx * dx + dy * dy;

        // If clicked inside
        if( (x - originalX)*(x - originalX) + (y - originalY)*(y - originalY) <= 50*50 ) {
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

        cout << "deltaX: " << deltaX << " ";
        cout << "deltaY: " << deltaY << "\n";

        circ.setCoord(circ.getX() + (deltaX/500), circ.getY() + (-deltaY/500), 0);
        glutPostRedisplay();
    } else if(circ.getResizeState()) {
        // Update history
        xHistory[0] = xHistory[1];
        yHistory[0] = yHistory[1];

        xHistory[1] = x;
        yHistory[1] = y;

        // cout << "Previous: " << xHistory[0] << " " << yHistory[0] << "\n";
        // cout << "Current: " << xHistory[1] << " " << yHistory[1] << "\n";

        double deltaX = xHistory[1] - xHistory[0];
        double deltaY = yHistory[1] - yHistory[0];
        double distante = deltaX + deltaY;

        cout << "deltaX: " << deltaX << " ";
        cout << "deltaY: " << deltaY << "\n";

        circ.setRadius(circ.getRadius() + distante/1000);
        glutPostRedisplay();
    }
}

void init(void) {
    /* selecionar cor de fundo (preto) */
    glClearColor (win.getRed(), win.getGreen(), win.getBlue(), 0.0);
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
    // win.setRGB(1.0, 1.0, 0.0);
    // readConfigFile("config.xml");
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
    /* C ANSI requer que main retorne um inteiro */
    return 0;
}