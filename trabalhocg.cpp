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

#define MOVEMENT 2
#define ANIMATION_FRAMES 30
#define ANIMATION_TIME 2000
#define FACTOR 0.5

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

    // Tall obstacles
    for (Circle o : obstacles) {
        o.draw();
    }

    // Short obstacles
    for (Circle lo : lowObstacles) {
        lo.draw();
    }

    // The player
    player.draw();

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

void ableToMove(double dx, double dy, double dz) {
    // Center collision
    if (player.collision(&center, dx/2, dy)) {
        return;
    }

    // Arena
    if( (sqrt(pow(arena.getX() - (player.getX() + dx), 2) + pow(arena.getY() - (player.getY() + dy), 2))
                                                             + player.getRadius()) >= arena.getRadius()) {
        return;
    }

    // Red obstacles
    for (Circle o : obstacles) {
        if (player.collision(&o, dx/2, dy)) {
            return;
        }
    }

    // Black obstacles
    for (Circle lo : lowObstacles) {
        if (player.collision(&lo, dx/2, dy)  && !player.isJumping()) {
            return;
        }
    }

    // If passes the previous filters, the player will actually move
    player.move(dx, dy, dz);
}

void jumpStart(int value) {
    player.setJumping(true);
    player.setRadius(player.getRadius() + player.getRadius() * (FACTOR/ANIMATION_FRAMES));
}

void jumpEnd(int value) {
    player.setRadius(player.getRadius() - player.getRadius() * (FACTOR/ANIMATION_FRAMES)); 
}

void onKeyDown(unsigned char key, int x, int y)
{
    switch (key) {
        case 'w':
        case 'W':
            keyStatus[(int) ('w')] = true;
            break;
        case 's':
        case 'S':
            keyStatus[(int) ('s')] = true;
            break;
        case 'a':
        case 'A':
             keyStatus[(int) ('a')] = true;
             break;
        case 'd':
        case 'D':
             keyStatus[(int) ('d')] = true;
             break;
        case 'p':
        case 'P':
            if (!player.isJumping()) {

                for (int i = 1; i <= ANIMATION_FRAMES; i++) {
                    glutTimerFunc( ((ANIMATION_TIME/2) / ANIMATION_FRAMES) * i, jumpStart, 0);
                }

                for (int i = 1; i <= ANIMATION_FRAMES; i++) {
                    glutTimerFunc(ANIMATION_TIME/2 + ((ANIMATION_TIME/2) / ANIMATION_FRAMES) * i, jumpEnd, 0);
                }

                // Hold on for 2 seconds
                glutTimerFunc(ANIMATION_TIME, [](int val) { player.setJumping(false); }, 0);
            }
            break;
    }

}

void onKeyUp(unsigned char key, int x, int y) {
    keyStatus[(int) key] = false;
}

void idle(void)
{
    if (keyStatus[ (int) ('a') ]) {
        ableToMove(- MOVEMENT, 0, 0);
    }

    if (keyStatus[ (int) ('d') ]) {
        ableToMove(MOVEMENT, 0, 0);
    }

    if (keyStatus[ (int) ('s') ]) {
        ableToMove(0, MOVEMENT, 0);
    }

    if (keyStatus[ (int) ('w') ]) {
        ableToMove(0, - MOVEMENT, 0);
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

    for (XMLElement* e = svg->FirstChildElement("circle"); e != NULL; e = e->NextSiblingElement("circle"))
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