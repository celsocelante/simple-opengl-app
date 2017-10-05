#include <iostream>
#include <string>
#include <math.h>
#include <list>
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
using namespace tinyxml2;

Circle arena;
Circle center;
Circle player;

// Obstacles (red ones)
list<Circle> obstacles;

// Low Obstacles (black ones)
list<Circle> lowObstacles;

// Window object
Window win;

// Key status
bool keyStatus[256] = { false };


void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);    
    
    // Fixed elements
    arena.draw();
    center.draw();
    player.draw();

    // Draw all the obstacles
    for (Circle o : obstacles) {
        o.draw();
    }

    for (Circle lo : lowObstacles) {
        lo.draw();
    }

    glFlush();
}

void init(void) {
    glClearColor(1, 1, 1, 0.0f);
    glViewport (0, 0, win.getWidth(), win.getHeight());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(
        (arena.getX() - arena.getRadius()),
        (arena.getX() + arena.getRadius()),
        (arena.getY() + arena.getRadius()),
        (arena.getY() - arena.getRadius()),
        -1.0, 1.0
    );
}

void onKeyUp(unsigned char key, int x, int y)
{
    keyStatus[ (int) (key) ] = false;
    glutPostRedisplay();
}

void onKeyDown(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'w':
        case 'W':
            player.move(0, -5);
            break;
        case 's':
        case 'S':
             player.move(0, 5);
             break;
        case 'a':
        case 'A':
             keyStatus[ (int) ('a') ] = true;
             break;
        case 'd':
        case 'D':
             keyStatus[ (int) ('d') ] = true;
             break;
    }
    glutPostRedisplay();
}

void idle(void)
{
    if (keyStatus[ (int) ('a') ]) {
        player.move(-1, 0);
    }

    if(keyStatus[ (int) ('d') ]) {
        player.move(1, 0);
    }
    
    glutPostRedisplay();
}

void readConfigFile(string fileName) {
    char path[200];
    char path2[200];

    strcpy(path, fileName.c_str());
    strcat(path, "config.xml");

    tinyxml2::XMLDocument doc;
    doc.LoadFile(path);

    cout << path << endl;

    XMLElement* app = doc.FirstChildElement("aplicacao");

    string nomeArquivo = app->FirstChildElement("arquivoDaArena")->Attribute("nome");
    string tipoArquivo = app->FirstChildElement("arquivoDaArena")->Attribute("tipo");
    string caminhoArquivo = app->FirstChildElement("arquivoDaArena")->Attribute("caminho");

    strcpy(path, "");
    strcpy(path, caminhoArquivo.c_str());
    strcat(path, "arena.svg");

    cout << path2 << endl;

    doc.LoadFile(path);
    XMLElement* svg = doc.FirstChildElement("svg");

    for(XMLElement* e = svg->FirstChildElement("circle"); e != NULL; e = e->NextSiblingElement("circle"))
    {
        double cx = e->DoubleAttribute("cx");
        double cy = e->DoubleAttribute("cy");
        double radius = e->DoubleAttribute("r");
        string fill = e->Attribute("fill");

        // Icone do jogador
        if (fill == "green") {
            player = Circle(cx, cy, 0, radius);
            player.setRGB(0, 1, 0);

        } else if (fill == "blue") {
            arena = Circle(cx, cy, 0, radius);
            arena.setRGB(0, 0, 1);

            win.setWidth((int) 2 * radius);
            win.setHeight((int) 2 * radius);
            win.setTitle("Arena");

        } else if (fill == "white") {
            center = Circle(cx, cy, 0, radius);
            center.setRGB(1, 1, 1);

        } else if (fill == "red") {
            Circle temp = Circle(cx, cy, 0, radius);
            temp.setRGB(1, 0, 0);
    
            obstacles.push_back(temp);

        } else if (fill == "black") {
            Circle temp = Circle(cx, cy, 0, radius);
            temp.setRGB(0, 0, 0);
    
            lowObstacles.push_back(temp);
        }
    }
}

int main(int argc, char** argv) {
    win = Window();

    if(argc == 2) {
        readConfigFile(argv[1]);
    } else {
        cerr << "Numero invalido de argumentos\n";
        exit(1);
    }

    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(win.getWidth(), win.getHeight());
    glutInitWindowPosition (100, 100);

    glutCreateWindow(win.getTitle().c_str());

    init();

    glutKeyboardFunc(onKeyDown);
    glutKeyboardUpFunc(onKeyUp);
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutMainLoop();

    return 0;
}