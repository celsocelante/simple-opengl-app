#include <iostream>
#include <string>
#include <math.h>
#include <list>
#include "tinyxml2/tinyxml2.h"
#include "Circle.h"
#include "Window.h"
#include "Robot.h"

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

// Robot object
Robot bot;

// Key status
bool keyStatus[256] = { false };

// If the player can walk on the low obstacles
bool canMoveFreely = false;

GLfloat mouseX = 0;

Circle disabledLowObstacle;


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
    // player.draw();

    bot.draw();

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

void ableToMove(GLfloat dx, GLfloat dy, GLfloat dz) {

    // Center collision
    if (player.collision(&center, dx, dy)) {
        return;
    }

    // Arena
    if( (sqrt(pow(arena.getX() - (player.getX() + dx), 2) + pow(arena.getY() - (player.getY() + dy), 2))
                                                             + player.getRadius()) >= arena.getRadius()) {
        return;
    }

    // Red obstacles
    for (Circle o : obstacles) {
        if (player.collision(&o, dx, dy)) {
            return;
        }
    }

    // Black obstacles
    GLint count = 0;

    if (!player.isJumping() && disabledLowObstacle.getId() != -1 && sqrt(pow(disabledLowObstacle.getX() - (player.getX() + dx), 2) + 
                                    pow(disabledLowObstacle.getY() - (player.getY() + dy), 2)) > (disabledLowObstacle.getRadius() + player.getRadius()) ) {
        canMoveFreely = false;
    }

    for (Circle lo : lowObstacles) {
        if (player.collision(&lo, dx, dy) && player.isJumping()) {
            canMoveFreely = true;

            // Last unlocked lowObstacle info
            disabledLowObstacle.setRadius(lo.getRadius());
            disabledLowObstacle.setX(lo.getX());
            disabledLowObstacle.setY(lo.getY());
            disabledLowObstacle.setId(lo.getId());
        }

        if (player.collision(&lo, dx, dy) && !player.isJumping() && !canMoveFreely) {
            return;
        }
    }

    // If passes the previous filters, the player will actually move
    player.move(dx, dy, dz);
}

void jumpStart(GLint value) {
    player.setJumping(true);
    GLfloat radius = player.getRadius();
    player.changeRadius(radius * (FACTOR/ANIMATION_FRAMES));
}

void jumpEnd(GLint value) {
    GLfloat radius = player.getRadius();
    player.changeRadius( -(radius * (FACTOR/ANIMATION_FRAMES)) ); 
}

void onKeyDown(unsigned char key, GLint x, GLint y)
{
    switch (key) {
        case 'w':
        case 'W':
            keyStatus[(GLint) ('w')] = true;
            break;
        case 's':
        case 'S':
            keyStatus[(GLint) ('s')] = true;
            break;
        case 'a':
        case 'A':
             keyStatus[(GLint) ('a')] = true;
             break;
        case 'd':
        case 'D':
             keyStatus[(GLint) ('d')] = true;
             break;
        case 'p':
        case 'P':
            if (!player.isJumping()) {

                for (GLint i = 1; i <= ANIMATION_FRAMES; i++) {
                    glutTimerFunc( ((ANIMATION_TIME/2) / ANIMATION_FRAMES) * i, jumpStart, 0);
                }

                for (GLint i = 1; i <= ANIMATION_FRAMES; i++) {
                    glutTimerFunc(ANIMATION_TIME/2 + ((ANIMATION_TIME/2) / ANIMATION_FRAMES) * i, jumpEnd, 0);
                }

                // Hold on for 2 seconds
                glutTimerFunc(ANIMATION_TIME, [](GLint val) { player.setJumping(false); player.restoreRadius(); }, 0);
            }
            break;
    }

}

void onKeyUp(unsigned char key, GLint x, GLint y) {
    keyStatus[(GLint) key] = false;
}

void idle(void)
{
    if (keyStatus[ (GLint) ('a') ]) {
        ableToMove(- MOVEMENT, 0, 0);
        bot.rotateLeft();
    }

    if (keyStatus[ (GLint) ('d') ]) {
        ableToMove(MOVEMENT, 0, 0);
        bot.rotateRight();
    }

    if (keyStatus[ (GLint) ('s') ]) {
        ableToMove(0, MOVEMENT, 0);
        bot.moveBackward();
    }

    if (keyStatus[ (GLint) ('w') ]) {
        ableToMove(0, - MOVEMENT, 0);
        bot.moveForward();
    }
    
    glutPostRedisplay();
}

void onPassiveMouseMotion(GLint x, GLint y) {
    if ((x >= 0 && x <= win.getWidth()) && (y >= 0 && y <= win.getHeight())) {

        GLfloat dx = x - mouseX;

        if (dx > 0) {
            bot.rotateArmRight();
            // cout << "Clockwise: " << dx << endl;
        } else if (dx < 0) {
            // cout << "Anti-Clockwise: " << dx << endl;
            bot.rotateArmLeft();
        }

        mouseX = x;
    }
}

void readConfigFile(string fileName) {
    char path[200];
    tinyxml2::XMLDocument doc;

    strcpy(path, fileName.c_str());
    strcat(path, "config.xml");
    doc.LoadFile(path);

    XMLElement* app = doc.FirstChildElement("aplicacao");

    string nomeArquivo = app->FirstChildElement("arquivoDaArena")->Attribute("nome");
    string tipoArquivo = app->FirstChildElement("arquivoDaArena")->Attribute("tipo");
    string caminhoArquivo = app->FirstChildElement("arquivoDaArena")->Attribute("caminho");

    GLfloat velTiro = app->FirstChildElement("jogador")->DoubleAttribute("velTiro");
    GLfloat vel = app->FirstChildElement("jogador")->DoubleAttribute("vel");

    strcpy(path, "");
    strcpy(path, caminhoArquivo.c_str());
    strcat(path, nomeArquivo.c_str());
    strcat(path, ".");
    strcat(path, tipoArquivo.c_str());
    doc.LoadFile(path);

    XMLElement* svg = doc.FirstChildElement("svg");

    for (XMLElement* e = svg->FirstChildElement("circle"); e != NULL; e = e->NextSiblingElement("circle"))
    {
        GLfloat cx = e->DoubleAttribute("cx");
        GLfloat cy = e->DoubleAttribute("cy");
        GLfloat radius = e->DoubleAttribute("r");
        GLint id = e->DoubleAttribute("id");
        string fill = e->Attribute("fill");

        // Icone do jogador
        if (fill == "green") {
            player = Circle(cx, cy, 0, radius);
            player.setRGB(0, 1, 0);
            player.setId(id);

            // Criar objeto do jogador
            bot = Robot(cx, cy, 0);

        } else if (fill == "blue") {
            arena = Circle(cx, cy, 0, radius);
            arena.setRGB(0, 0, 1);
            arena.setId(id);

            win.setWidth((GLint) 2 * radius);
            win.setHeight((GLint) 2 * radius);
            win.setTitle("Arena");

        } else if (fill == "white") {
            center = Circle(cx, cy, 0, radius);
            center.setRGB(1, 1, 1);
            center.setId(id);

        } else if (fill == "red") {
            Circle temp = Circle(cx, cy, 0, radius);
            temp.setRGB(1, 0, 0);
            temp.setId(id);
    
            obstacles.push_back(temp);

        } else if (fill == "black") {
            Circle temp = Circle(cx, cy, 0, radius);
            temp.setRGB(0, 0, 0);
            temp.setId(id);
    
            lowObstacles.push_back(temp);
        }
    }
}

GLint main(GLint argc, char** argv) {
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
    glutPassiveMotionFunc(onPassiveMouseMotion);
    glutIdleFunc(idle);

    glutMainLoop();

    return 0;
}