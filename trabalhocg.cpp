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

// Player
Circle player;

// Obstacles
list<Circle> obstacles;

Circle arena = Circle();

// Initial window object
Window win = Window();


void display(void) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(arena.getX() - arena.getRadius(),
        arena.getX() - arena.getRadius() + win.getWidth(),
        arena.getY() - arena.getRadius(),
        arena.getY() - arena.getRadius() + win.getHeight(), -1.0, 1.0
    );
    
    player.draw();
    arena.draw();

    // Draw all the obstacles
    for (Circle e : obstacles) {
        e.draw();
    }

    glFlush();
}

void init(void) {
    

    glClear(GL_COLOR_BUFFER_BIT);     
    glPushMatrix(); 
    glClearColor (win.getRed(), win.getGreen(), win.getBlue(), 0);

    glFlush();
}


void idle(void)
{   
    glutPostRedisplay();
}

void onMouseClick(int button, int state, int x, int y) {
    cout << x << ", " << y << endl;
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

        // cout << cx << ", " << cy << endl;

        // Icone do jogador
        if (fill == "green") {
            player = Circle(cx, cy, 0, radius);
            player.setRGB(0, 1, 0);

            continue;

        } else if (fill == "blue") {
            arena.setCoord(cx, cy, 0);
            arena.setRGB(0, 0, 1);

            win.setWidth(2 * radius);
            win.setHeight(2 * radius);
            win.setTitle("Arena");

            continue;
        }


        Circle temp = Circle(cx, cy, 0, radius);
        temp.setRGB(0, 0, 0);

        obstacles.push_back(temp);
    }
    
    // circ = Circle(0, 0, 0, raio);
    // circ.setRGB(corR, corG, corB);
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

    glutInitWindowSize(win.getWidth(), win.getHeight());
    glutInitWindowPosition (100, 100);
    glutCreateWindow(win.getTitle().c_str());

    init();

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMouseFunc(onMouseClick);

    glutMainLoop();

    return 0;
}