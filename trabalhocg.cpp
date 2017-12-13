#include <iostream>
#include <string>
#include <math.h>
#include <list>
#include "tinyxml2/tinyxml2.h"
#include "Window.h"
#include "Robot.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Stuff.h"

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

using namespace std;
using namespace tinyxml2;

Stuff* stuff; // hold all the shared objects

// Window object
Window win;

// Key status
bool keyStatus[256] = { false };

GLfloat mouseX = 0;
GLfloat lastTime = 0;
GLfloat lastTimeShot = 0;

//Camera controls
int currentCamera = 1;


void init(void) {
    glClearColor(1, 1, 1, 0.0f);
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
    glRasterPos2f(stuff->arena->getX() + stuff->arena->getX() * 0.4, stuff->arena->getY() - stuff->arena->getY() * 0.5);

    tmpStr = str;
    while( *tmpStr ){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *tmpStr);
        tmpStr++;
    }
}

void renderWinText() {
    static char str[2000];
    char *tmpStr;

    glLoadIdentity();
    glViewport(0,0 , win.getWidth(), win.getHeight() + 200);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(stuff->arena->getX() - stuff->arena->getRadius(), stuff->arena->getX() + stuff->arena->getRadius(),
        stuff->arena->getY() - stuff->arena->getRadius(),stuff->arena->getY() + stuff->arena->getRadius(), -1, 1);
    glMatrixMode(GL_MODELVIEW);

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

    glLoadIdentity();
    glViewport(0,0 , win.getWidth(), win.getHeight() + 200);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(stuff->arena->getX() - stuff->arena->getRadius(), stuff->arena->getX() + stuff->arena->getRadius(),
        stuff->arena->getY() - stuff->arena->getRadius(),stuff->arena->getY() + stuff->arena->getRadius(), -1, 1);
    glMatrixMode(GL_MODELVIEW);

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

        for (Enemy* enemy : stuff->enemies) {
            if ( enemy->displayed && enemy->collisionNoDist(b->getX(), b->getY(), SIZE_BULLET) ) {
                stuff->totalScore++;
                stuff->totalEnemies--;
                enemy->displayed = false;
                break;
            }
        }

        if (!stuff->arena->collisionNoDist(b->getX(), b->getY(), SIZE_BULLET) ||
            stuff->center->collisionNoDist(b->getX(), b->getY(), SIZE_BULLET)) {

            stuff->bullets.remove(b);
            return;
        }

        for (Circle* lo : stuff->obstacles) {
            if (lo->collisionNoDist(b->getX(), b->getY(), SIZE_BULLET)) {

                stuff->bullets.remove(b);
                return;
            }
        }
    }


    for (Bullet* b: stuff->enemyBullets) {
        b->draw();
        b->update(elapsedTime);

        if ( stuff->bot->collisionNoDist(b->getX(), b->getY(), SIZE_BULLET) ) {
            stuff->bot->displayed = false;
            break;
        }


        for (Circle* lo : stuff->obstacles) {
            if ((lo->collisionNoDist(b->getX(), b->getY(), SIZE_BULLET)) ||
                (!stuff->arena->collisionNoDist(b->getX(), b->getY(), SIZE_BULLET)) ||
                    stuff->center->collisionNoDist(b->getX(), b->getY(), SIZE_BULLET)) {

                stuff->enemyBullets.remove(b);
                return;
            }
        }
    }
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
            // stuff->bot->jump();

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

            case '1':
                currentCamera = 1;
                cout << "camera 1" << endl;
                break;
            case '2':
                currentCamera = 2;
                cout << "camera 2" << endl;
    }

}

void onKeyUp(unsigned char key, GLint x, GLint y) {
    keyStatus[(GLint) key] = false;

    if (stuff->gameEnded) {
        exitProgram();
    }
}

void idle(void) {
    // glEnable(GL_DEPTH_TEST);
    // glEnable( GL_TEXTURE_2D );
    // glEnable(GL_LIGHTING);
    // glShadeModel (GL_SMOOTH);
    // glDepthFunc(GL_LEQUAL);

    if (keyStatus[ (GLint) ('a') ]) {
        stuff->bot->rotateLeft();
    }

    if (keyStatus[ (GLint) ('d') ]) {
        stuff->bot->rotateRight();
    }

    if (keyStatus[ (GLint) ('s') ]) {
        stuff->bot->moveBackward();
    }

    if (keyStatus[ (GLint) ('w') ]) {
        stuff->bot->moveForward();
    }


    glutPostRedisplay();
}

void onPassiveMouseMotion(GLint x, GLint y) {
    if ((x >= 0 && x <= win.getWidth()) && (y >= 0 && y <= win.getHeight())) {

        GLfloat dx = x - mouseX;

        if (dx > 0) {
            stuff->bot->rotateArmLeft();
        } else if (dx < 0) {
            stuff->bot->rotateArmRight();
        }

        mouseX = x;
    }
}

void onClick(GLint button, GLint state, GLint x, GLint y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        stuff->bot->setFire();
    }
}

int dist = 20;

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor (1.0, 1.0, 1.0, 1.0);

    for (int i = 0; i < 3; i++) {
        if (i == 0) {
            if (currentCamera == 1) {
                // primeira camera
                glViewport(0, 0, win.getWidth(), win.getHeight());
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluPerspective(45, win.getHeight() / win.getWidth(), 2, 2000);
                glMatrixMode(GL_MODELVIEW);
                gluLookAt(stuff->bot->getX(), stuff->bot->getY(), 100, 500, 500, 0, 0, 0, 1);
            } else if (currentCamera == 2) {
                // segunda camera
                glViewport(0, 0, win.getWidth(), win.getHeight());
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluPerspective(45, win.getHeight() / win.getWidth(), 2, 2000);
                glMatrixMode(GL_MODELVIEW);
                gluLookAt(stuff->bot->getX(), stuff->bot->getY(), 100, 500, 500, 0, 0, 0, 1);
            }
        }
        else if (i == 1) {
            glLoadIdentity();
            glViewport(0, 500, win.getWidth(), 200);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(45, win.getWidth() / 200, 2, 1000);
            glMatrixMode(GL_MODELVIEW);
            gluLookAt(stuff->bot->getX(), stuff->bot->getY(), 100, stuff->bot->getX() + cos((stuff->bot->getTheta() + 90) * M_PI / 180),
                stuff->bot->getY() + sin((stuff->bot->getTheta() + 90) * M_PI / 180), 100, 0, 0, 1);
            
        } else if (i == 2) {
            // minimapa
            glLoadIdentity();
            glViewport(375, 0, win.getWidth()/4, win.getHeight()/4);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(stuff->arena->getX() - stuff->arena->getRadius(),stuff->arena->getX() + stuff->arena->getRadius(),
                stuff->arena->getY() - stuff->arena->getRadius(),stuff->arena->getY() + stuff->arena->getRadius(),-20.0,20.0);
        }


        if (stuff->totalEnemies == 0) {
            glClearColor (0, 0, 0, 1.0);
            // render final text
            renderWinText();
            glFlush();
    
            return;
        }
    
        if (!stuff->bot->displayed) {
            glClearColor (0, 0, 0, 1.0);
            // render final text
            renderGameOverText();
            glFlush();
    
            return;
        }
    
        // Fixed elements
        stuff->arena->draw();
    
        // bullets
        drawBullets();
    
        stuff->center->draw();
    
        // Short obstacles
        for (Circle* lo : stuff->obstacles) {
            lo->draw();
        }
    
        renderScoreText();
    
        // Enemies
        for (Enemy* e : stuff->enemies) {
            if (e->displayed) {
                e->draw();
                e->update();
    
                GLfloat atual = glutGet(GLUT_ELAPSED_TIME);
                if (e->lastTimeShot == 0 || (atual - e->lastTimeShot) / 1000 > 1 / e->freqTiro){
                    e->setFire();
                    e->lastTimeShot = atual;
                }
            }
        }
    
        stuff->bot->draw();

    }

    glutSwapBuffers();
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


    GLfloat velTiroInimigo = app->FirstChildElement("inimigo")->DoubleAttribute("velTiro");
    GLfloat velInimigo = app->FirstChildElement("inimigo")->DoubleAttribute("vel");
    GLfloat freqTiro = app->FirstChildElement("inimigo")->DoubleAttribute("freqTiro");

    GLfloat alturaObstaculo = app->FirstChildElement("obstaculo")->DoubleAttribute("altura");

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
            stuff->bot->setHeight(80); // altura do robo

        } else if (fill == "blue") {
            stuff->arena = new Circle(cx, cy, 0, radius, 2000);
            stuff->arena->setRGB(0, 0, 1);
            stuff->arena->setId(id);
            stuff->arena->setStuff(stuff);

            win.setWidth((GLint) 2 * radius);
            win.setHeight((GLint) 2 * radius);
            win.setTitle("Arena");

        } else if (fill == "white") {
            stuff->center = new Circle(cx, cy, 0, radius, 2000);
            stuff->center->setRGB(1, 1, 1);
            stuff->center->setId(id);
            stuff->center->setStuff(stuff);

        } else if (fill == "red") {
            Enemy* temp = new Enemy(cx, cy, 0, radius);
            temp->setVelocity(velInimigo);
            temp->setBulletVelocity(velTiroInimigo);
            temp->setId(id);
            temp->setStuff(stuff);
            temp->freqTiro = freqTiro;

            stuff->enemies.push_back(temp);

            // count enemies
            stuff->totalEnemies++;

        } else if (fill == "black") {
            Circle* temp = new Circle(cx, cy, 0, radius, 20);
            temp->setRGB(0, 0, 0);
            temp->setId(id);

            stuff->obstacles.push_back(temp);
        }
    }
}

GLint main(GLint argc, char** argv) {
    win = Window();
    stuff = new Stuff();

    if (argc == 2) {
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
