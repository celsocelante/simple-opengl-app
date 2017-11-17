#include <iostream>
#include <string>
#include <math.h>
#include <list>
#include "tinyxml2/tinyxml2.h"
#include "Circle.h"
#include "Window.h"
#include "Robot.h"
#include "Bullet.h"
#include "Enemy.h"

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif

#define ANIMATION_FRAMES 30
#define ANIMATION_TIME 2000
#define FACTOR 0.5

using namespace std;
using namespace tinyxml2;

Circle arena;
Circle center;
Circle player;
Circle disabledLowObstacle;

// Obstacles (red ones)
list<Enemy> obstacles;

// Low Obstacles (black ones)
list<Circle> lowObstacles;

// Bullets by robot
list<Bullet*> bullets;

// Bullets by enemies
list<Bullet*> enemyBullets;

// Window object
Window win;

// Robot object
Robot bot;

// Key status
bool keyStatus[256] = { false };

GLfloat mouseX = 0;
GLfloat lastTime = 0;
GLint totalScore = 0;

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

void renderText(GLfloat x, GLfloat y) {
    static char str[2000];
    char *tmpStr;
    sprintf(str, "Score: %d", totalScore);
    glColor3f(1, 0, 0);
    glRasterPos2f(x, y);

    tmpStr = str;
    while( *tmpStr ){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *tmpStr);
        tmpStr++;
    }
}

void drawBullets() {
    GLfloat currentTime = glutGet(GLUT_ELAPSED_TIME);
    GLfloat elapsedTime = currentTime - lastTime;
    lastTime = currentTime;

    for (Bullet* b: bullets) {
        b->draw();
        b->update(elapsedTime);

        for (Circle lo : lowObstacles) {
            if ((lo.collisionNoDist(b->getX(), b->getY(), SIZE_BULLET)) ||
                (!b->collision(&arena, 10, 10)) ||
                b->collision(&center, 10, 10)) {

                bullets.remove(b);
                return;
            }
        }
    }
}

bool ableToMove(GLfloat dx, GLfloat dy, GLfloat dz) {

    // Center collision
    if (bot.collision(&center, dx, dy)) {
        return false;
    }

    // Arena
    if( (sqrt(pow(arena.getX() - (bot.getX() + dx), 2) + pow(arena.getY() - (bot.getY() + dy), 2))
                                                             + bot.getRadius()) >= arena.getRadius()) {
        return false;
    }

    // Red obstacles
    for (Circle o : obstacles) {
        if (bot.collision(&o, dx, dy)) {
            return false;
        }
    }

    // Black obstacles
    if (!bot.isJumping() && disabledLowObstacle.getId() != -1 && sqrt(pow(disabledLowObstacle.getX() - (bot.getX() + dx), 2) + 
                                    pow(disabledLowObstacle.getY() - (bot.getY() + dy), 2)) > (disabledLowObstacle.getRadius() + bot.getRadius()) ) {
        bot.setMoveFreely(false);
    }

    for (Circle lo : lowObstacles) {
        if (bot.collision(&lo, dx, dy) && bot.isJumping()) {
            bot.setMoveFreely(true);

            // Last unlocked lowObstacle info
            disabledLowObstacle.setRadius(lo.getRadius());
            disabledLowObstacle.setX(lo.getX());
            disabledLowObstacle.setY(lo.getY());
            disabledLowObstacle.setId(lo.getId());
        }

        if (bot.collision(&lo, dx, dy) && !bot.isJumping() && !bot.canMoveFreely()) {
            return false;
        }
    }

    return true;
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
            if (!bot.isJumping()) {
                for (GLint i = 1; i <= ANIMATION_FRAMES; i++) {
                    glutTimerFunc( ((ANIMATION_TIME/2) / ANIMATION_FRAMES) * i, [](GLint v) {
                        bot.setJumping(true);
                        GLfloat radius = bot.getRadius();
                        GLfloat scale = bot.getScale();
                    
                        bot.changeRadius(radius * (FACTOR/ANIMATION_FRAMES));
                        bot.changeScale(FACTOR/ANIMATION_FRAMES);
                    }, 0);
                }

                for (GLint i = 1; i <= ANIMATION_FRAMES; i++) {
                    glutTimerFunc(ANIMATION_TIME/2 + ((ANIMATION_TIME/2) / ANIMATION_FRAMES) * i, [](GLint v) {
                        GLfloat radius = bot.getRadius();
                        GLfloat scale = bot.getScale();
                    
                        bot.changeRadius( -(radius * (FACTOR/ANIMATION_FRAMES)) ); 
                        bot.changeScale(-((FACTOR/ANIMATION_FRAMES)));
                    }, 0);
                }

                // Hold on for 2 seconds
                glutTimerFunc(ANIMATION_TIME, [](GLint v) {
                    bot.setJumping(false);
                    bot.restoreRadius();
                    bot.restoreScale();
                }, 0);
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
        bot.rotateLeft();
    }

    if (keyStatus[ (GLint) ('d') ]) {
        bot.rotateRight();
    }

    if (keyStatus[ (GLint) ('s') ]) {
        GLfloat newX = bot.getX() + bot.newX();
        GLfloat newY = bot.getY() + bot.newY();

        if (ableToMove(newX - bot.getX(), newY - bot.getY(), 0)) {
            bot.moveBackward();
        }
    }

    if (keyStatus[ (GLint) ('w') ]) {
        GLfloat newX = bot.getX() - bot.newX();
        GLfloat newY = bot.getY() - bot.newY();


        if (ableToMove(newX - bot.getX(), newY - bot.getY(), 0)) {
            bot.moveForward();
        }
    }

    
    glutPostRedisplay();
}

void onPassiveMouseMotion(GLint x, GLint y) {
    if ((x >= 0 && x <= win.getWidth()) && (y >= 0 && y <= win.getHeight())) {

        GLfloat dx = x - mouseX;

        if (dx > 0) {
            bot.rotateArmLeft();
            // cout << "Clockwise: " << dx << endl;
        } else if (dx < 0) {
            // cout << "Anti-Clockwise: " << dx << endl;
            bot.rotateArmRight();
        }

        mouseX = x;
    }
}

void onClick(GLint button, GLint state, GLint x, GLint y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && !bot.canMoveFreely() && !bot.isJumping()) {
        bullets.push_back(new Bullet(bot.getX(), bot.getY(), bot.getThetaArm(), 
            bot.getTheta(), 0.5, bot.getRadius()));
    }
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);    
    
    // Fixed elements
    arena.draw();

    // bullets
    drawBullets();

    // Tall obstacles
    for (Enemy o : obstacles) {
        o.draw();
    }

    center.draw();

    // Short obstacles
    for (Circle lo : lowObstacles) {
        lo.draw();
    }

    bot.draw();

    // Score
    renderText(700, 250);

    glFlush();
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
            // Criar objeto do jogador
            bot = Robot(cx, cy, 0, radius);
            bot.setId(id);
            bot.setVelocity(vel);
            bot.setBulletVelocity(velTiro);

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
            Enemy temp = Enemy(cx, cy, 0, radius);
            temp.setVelocity(vel);
            temp.setBulletVelocity(velTiro);
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
    glutMouseFunc(onClick);
    glutPassiveMotionFunc(onPassiveMouseMotion);
    glutIdleFunc(idle);

    glutMainLoop();

    return 0;
}