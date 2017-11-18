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
#include "Stuff.h"

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

Stuff* stuff; // hold all the shared objects

Circle disabledLowObstacle;

// Window object
Window win;

// Robot object
Robot* bot;

// Key status
bool keyStatus[256] = { false };

GLfloat mouseX = 0;
GLfloat lastTime = 0;

void init(void) {
    glClearColor(1, 1, 1, 0.0f);
    glViewport (0, 0, win.getWidth(), win.getHeight());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(
        (stuff->arena->getX() - stuff->arena->getRadius()),
        (stuff->arena->getX() + stuff->arena->getRadius()),
        (stuff->arena->getY() + stuff->arena->getRadius()),
        (stuff->arena->getY() - stuff->arena->getRadius()),
        -1.0, 1.0
    );
}

void endGame() {
    // Change stante to finished
    stuff->gameEnded = true;
}

void exitProgram() {
    exit(0);
}

void renderScoreText() {
    static char str[2000];
    char *tmpStr;
    sprintf(str, "Score: %d", stuff->totalScore);
    glColor3f(1, 0, 0);
    glRasterPos2f(200, 200);

    tmpStr = str;
    while( *tmpStr ){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *tmpStr);
        tmpStr++;
    }
}

void renderWinText() {
    static char str[2000];
    char *tmpStr;
    sprintf(str, "YOU WIN");
    glColor3f(0, 1, 0);
    glRasterPos2f(stuff->arena->getX() - stuff->arena->getX() * 0.05, stuff->arena->getY());

    tmpStr = str;
    while( *tmpStr ){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *tmpStr);
        tmpStr++;
    }

    sprintf(str, "Press any key to exit");
    glColor3f(1, 1, 1);
    glRasterPos2f(stuff->arena->getX() - stuff->arena->getX() * 0.14, stuff->arena->getY() + stuff->arena->getY() * 0.1);

    tmpStr = str;
    while( *tmpStr ){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *tmpStr);
        tmpStr++;
    }

    endGame();
}

void renderGameOverText() {
    static char str[2000];
    char *tmpStr;
    sprintf(str, "GAME OVER");
    glColor3f(1, 0, 0);
    glRasterPos2f(stuff->arena->getX() - stuff->arena->getX() * 0.05, stuff->arena->getY());

    tmpStr = str;
    while( *tmpStr ){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *tmpStr);
        tmpStr++;
    }

    sprintf(str, "Press any key to exit");
    glColor3f(1, 1, 1);
    glRasterPos2f(stuff->arena->getX() - stuff->arena->getX() * 0.14, stuff->arena->getY() + stuff->arena->getY() * 0.1);

    tmpStr = str;
    while( *tmpStr ){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *tmpStr);
        tmpStr++;
    }

    endGame();
}

void drawBullets() {
    GLfloat currentTime = glutGet(GLUT_ELAPSED_TIME);
    GLfloat elapsedTime = currentTime - lastTime;
    lastTime = currentTime;

    for (Bullet* b: stuff->bullets) {
        b->draw();
        b->update(elapsedTime);

        for (Circle* lo : stuff->obstacles) {
            if ((lo->collisionNoDist(b->getX(), b->getY(), SIZE_BULLET)) ||
                (!b->collision(stuff->arena, 10, 10)) ||
                b->collision(stuff->center, 10, 10)) {

                stuff->bullets.remove(b);
                return;
            }
        }
    }
}

bool ableToMove(GLfloat dx, GLfloat dy, GLfloat dz) {
    // Center collision
    if (stuff->bot->collision(stuff->center, dx, dy)) {
        return false;
    }

    // Arena
    if( (sqrt(pow(stuff->arena->getX() - (stuff->bot->getX() + dx), 2) + pow(stuff->arena->getY() - (stuff->bot->getY() + dy), 2))
                                                             + stuff->bot->getRadius()) >= stuff->arena->getRadius()) {
        return false;
    }

    // Red obstacles
    for (Circle* o : stuff->enemies) {
        if (stuff->bot->collision(o, dx, dy)) {
            return false;
        }
    }

    // Black obstacles
    if (!stuff->bot->isJumping() && disabledLowObstacle.getId() != -1 && sqrt(pow(disabledLowObstacle.getX() - (stuff->bot->getX() + dx), 2) + 
                                    pow(disabledLowObstacle.getY() - (stuff->bot->getY() + dy), 2)) > (disabledLowObstacle.getRadius() + stuff->bot->getRadius()) ) {
        stuff->bot->setMoveFreely(false);
    }

    for (Circle* lo : stuff->obstacles) {
        if (stuff->bot->collision(lo, dx, dy) && stuff->bot->isJumping()) {
            stuff->bot->setMoveFreely(true);

            // Last unlocked lowObstacle info
            disabledLowObstacle.setRadius(lo->getRadius());
            disabledLowObstacle.setX(lo->getX());
            disabledLowObstacle.setY(lo->getY());
            disabledLowObstacle.setId(lo->getId());
        }

        if (stuff->bot->collision(lo, dx, dy) && !stuff->bot->isJumping() && !stuff->bot->canMoveFreely()) {
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
            if (!stuff->bot->isJumping()) {
                for (GLint i = 1; i <= ANIMATION_FRAMES; i++) {
                    glutTimerFunc( ((ANIMATION_TIME/2) / ANIMATION_FRAMES) * i, [](GLint v) {
                        stuff->bot->setJumping(true);
                        GLfloat radius = stuff->bot->getRadius();
                        GLfloat scale = stuff->bot->getScale();
                    
                        stuff->bot->changeRadius(radius * (FACTOR/ANIMATION_FRAMES));
                        stuff->bot->changeScale(FACTOR/ANIMATION_FRAMES);
                    }, 0);
                }

                for (GLint i = 1; i <= ANIMATION_FRAMES; i++) {
                    glutTimerFunc(ANIMATION_TIME/2 + ((ANIMATION_TIME/2) / ANIMATION_FRAMES) * i, [](GLint v) {
                        GLfloat radius = stuff->bot->getRadius();
                        GLfloat scale = stuff->bot->getScale();
                    
                        stuff->bot->changeRadius( -(radius * (FACTOR/ANIMATION_FRAMES)) ); 
                        stuff->bot->changeScale(-((FACTOR/ANIMATION_FRAMES)));
                    }, 0);
                }

                // Hold on for 2 seconds
                glutTimerFunc(ANIMATION_TIME, [](GLint v) {
                    stuff->bot->setJumping(false);
                    stuff->bot->restoreRadius();
                    stuff->bot->restoreScale();
                }, 0);
            }
            break;
    }

}

void onKeyUp(unsigned char key, GLint x, GLint y) {
    keyStatus[(GLint) key] = false;

    if (stuff->gameEnded) {
        exitProgram();
    }
}

void idle(void)
{

    if (keyStatus[ (GLint) ('a') ]) {
        stuff->bot->rotateLeft();
    }

    if (keyStatus[ (GLint) ('d') ]) {
        stuff->bot->rotateRight();
    }

    if (keyStatus[ (GLint) ('s') ]) {
        GLfloat newX = stuff->bot->getX() + stuff->bot->newX();
        GLfloat newY = stuff->bot->getY() + stuff->bot->newY();

        if (ableToMove(newX - stuff->bot->getX(), newY - stuff->bot->getY(), 0)) {
            stuff->bot->moveBackward();
        }
    }

    if (keyStatus[ (GLint) ('w') ]) {
        GLfloat newX = stuff->bot->getX() - stuff->bot->newX();
        GLfloat newY = stuff->bot->getY() - stuff->bot->newY();


        if (ableToMove(newX - stuff->bot->getX(), newY - stuff->bot->getY(), 0)) {
            stuff->bot->moveForward();
        }
    }

    
    glutPostRedisplay();
}

void onPassiveMouseMotion(GLint x, GLint y) {
    if ((x >= 0 && x <= win.getWidth()) && (y >= 0 && y <= win.getHeight())) {

        GLfloat dx = x - mouseX;

        if (dx > 0) {
            stuff->bot->rotateArmLeft();
            // cout << "Clockwise: " << dx << endl;
        } else if (dx < 0) {
            // cout << "Anti-Clockwise: " << dx << endl;
            stuff->bot->rotateArmRight();
        }

        mouseX = x;
    }
}

void onClick(GLint button, GLint state, GLint x, GLint y) {
    if (stuff->gameEnded) {
        exitProgram();
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && !stuff->bot->canMoveFreely() && !stuff->bot->isJumping()) {
        stuff->bullets.push_back(new Bullet(stuff->bot->getX(), stuff->bot->getY(), stuff->bot->getThetaArm(), 
            stuff->bot->getTheta(), 0.5, stuff->bot->getRadius()));
    }
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    if (stuff->totalEnemies == 0) {
        glClearColor(0, 0, 0, 0.0f);
        // render final text
        renderWinText();
        glFlush();

        return;
    } 
    
    // Fixed elements
    stuff->arena->draw();

    // bullets
    drawBullets();

    // Tall obstacles
    for (Enemy* e : stuff->enemies) {
        e->draw();
    }

    stuff->center->draw();

    // Short obstacles
    for (Circle* lo : stuff->obstacles) {
        lo->draw();
    }

    stuff->bot->draw();

    // Score
    renderScoreText();

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
            stuff->bot = new Robot(cx, cy, 0, radius);
            stuff->bot->setId(id);
            stuff->bot->setVelocity(vel);
            stuff->bot->setBulletVelocity(velTiro);
            stuff->bot->setStuff(stuff);

        } else if (fill == "blue") {
            stuff->arena = new Circle(cx, cy, 0, radius);
            stuff->arena->setRGB(0, 0, 1);
            stuff->arena->setId(id);
            stuff->arena->setStuff(stuff);

            win.setWidth((GLint) 2 * radius);
            win.setHeight((GLint) 2 * radius);
            win.setTitle("Arena");

        } else if (fill == "white") {
            stuff->center = new Circle(cx, cy, 0, radius);
            stuff->center->setRGB(1, 1, 1);
            stuff->center->setId(id);
            stuff->center->setStuff(stuff);

        } else if (fill == "red") {
            Enemy* temp = new Enemy(cx, cy, 0, radius);
            temp->setVelocity(vel);
            temp->setBulletVelocity(velTiro);
            temp->setId(id);
            temp->setStuff(stuff);
    
            stuff->enemies.push_back(temp);

            // count enemies
            stuff->totalEnemies++;

        } else if (fill == "black") {
            Circle* temp = new Circle(cx, cy, 0, radius);
            temp->setRGB(0, 0, 0);
            temp->setId(id);
    
            stuff->obstacles.push_back(temp);
        }
    }
}

GLint main(GLint argc, char** argv) {
    win = Window();
    stuff = new Stuff();

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