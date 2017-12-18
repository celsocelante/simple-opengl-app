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
#include "TextureLoader.h"

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#define ALT_JOGADOR 2.5 * 20

using namespace std;
using namespace tinyxml2;

Stuff* stuff; // hold all the shared objects

// Window object
Window win;

// Key status
bool keyStatus[256] = { false };

GLfloat mouseX = 0;
GLfloat mouseY = 0;
GLfloat lastTime = 0;
GLfloat lastTimeShot = 0;

GLfloat camXYangle = 0;
GLfloat camZangle = 25;
GLfloat camDist = 150;

//Camera controls
int currentCamera = 1;

// Night mode
bool nightMode = false;


void init(void) {
    glClearColor(1, 1, 1, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable( GL_TEXTURE_2D );
    glEnable(GL_LIGHTING);
    glShadeModel (GL_SMOOTH);

    GLfloat light_position0[] = { stuff->arena->getX()+stuff->center->getRadius()+((stuff->arena->getRadius() - stuff->center->getRadius())/2),
        stuff->arena->getY(), 40, 1.0 };

    GLfloat ambientfactor = 0.0;
    GLfloat diffusefactor = 1.5;

    GLfloat ambientLight[] = { ambientfactor, ambientfactor, ambientfactor, 1.0f };
    GLfloat diffuseLight[] = { diffusefactor, diffusefactor, diffusefactor, 1.0};
    GLfloat specularLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION , 0.001f);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position0);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION , 0.001f);
    glEnable(GL_LIGHT1);

    glLightfv(GL_LIGHT2, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT2, GL_POSITION, light_position0);
    glLightfv(GL_LIGHT2, GL_SPECULAR, specularLight);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 5.0);// set cutoff angle
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_position0);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION , 0.001f);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 1); // set focusing strength
}


void RasterChars(GLfloat x, GLfloat y, GLfloat z, const char * text, double r, double g, double b)
{
    //Push to recover original attributes
    glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        //Draw text in the x, y, z position
        glColor3f(r, g, b);
        glRasterPos3f(x, y, z);
        const char* tmpStr;
        tmpStr = text;
        while( *tmpStr ){
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *tmpStr);
            tmpStr++;
        }
    glPopAttrib();
}

void PrintText(GLfloat x, GLfloat y, const char * text, double r, double g, double b)
{
    //Draw text considering a 2D space (disable all 3d features)
    glMatrixMode (GL_PROJECTION);
    //Push to recover original PROJECTION MATRIX
    glPushMatrix();
        glLoadIdentity ();
        glOrtho(
        (stuff->arena->getX() - stuff->arena->getRadius()),
        (stuff->arena->getX() + stuff->arena->getRadius()),
        (stuff->arena->getY() + stuff->arena->getRadius()),
        (stuff->arena->getY() - stuff->arena->getRadius()),
        -1.0, 1.0
        );
        RasterChars(x, y, 0, text, r, g, b);    
    glPopMatrix();
    glMatrixMode (GL_MODELVIEW);
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

    glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glRasterPos2f(stuff->arena->getX() + stuff->arena->getX() * 0.4, stuff->arena->getY() - stuff->arena->getY() * 0.5);

        tmpStr = str;
        while( *tmpStr ){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *tmpStr);
            tmpStr++;
        }
    glPopAttrib();
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

    }


    for (Bullet* b: stuff->enemyBullets) {
        b->draw();
        b->update(elapsedTime);

        if ( stuff->bot->collisionNoDist(b->getX(), b->getY(), SIZE_BULLET) ) {
            stuff->bot->displayed = false;
            break;
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
        
        case 'n':
        case 'N':
            nightMode = !nightMode;
            cout << "Night mode " << (nightMode ? "on" : "off") << endl;
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
            break;
    }

}

void onKeyUp(unsigned char key, GLint x, GLint y) {
    keyStatus[(GLint) key] = false;

    if (stuff->gameEnded) {
        exitProgram();
    }
}

void idle(void) {

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
        GLfloat dy = y - mouseY;

        // horizontal movement
        if (dx > 0) {
            stuff->bot->rotateArmLeft();
        } else if (dx < 0) {
            stuff->bot->rotateArmRight();
        }

        // vertical movement
        if (dy > 0) {
            stuff->bot->rotateArmDown();
        } else if (dy < 0) {
            stuff->bot->rotateArmUp();
        }

        mouseX = x;
        mouseY = y;
    }
}

void onClick(GLint button, GLint state, GLint x, GLint y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        stuff->bot->setFire();
    }
}


void render(int i) {

    if (i == 3) {
        return;
    }

    // Minimap
    if (i == 2) {
        glDisable(GL_LIGHTING);
        glDisable( GL_TEXTURE_2D );

        // Fixed elements
        stuff->arena->drawMinimap();

        // Enemies
        for (Enemy* e : stuff->enemies) {
            if (e->displayed) {
                e->drawMinimap();
            }
        }

        stuff->bot->drawMinimap();

        for (Circle* lo : stuff->obstacles) {
            lo->drawObstacle2d();
        }

        stuff->center->drawMinimap();

        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
    } else {
        // Fixed elements
        stuff->arena->draw();


        // Enemies
        for (Enemy* e : stuff->enemies) {
            if (e->displayed) {
                e->draw();
                e->update(); // movimentacao dos inimigos

                GLfloat atual = glutGet(GLUT_ELAPSED_TIME);
                if (e->lastTimeShot == 0 || (atual - e->lastTimeShot) / 1000 > 1 / e->freqTiro){
                    e->setFire();
                    e->lastTimeShot = atual;
                }
            }
        }

        stuff->bot->draw(i);


        // Short obstacles
        for (Circle* lo : stuff->obstacles) {
            lo->drawObstacle(stuff->obstaclesTexture);
        }

        // bullets
        drawBullets();

        stuff->center->draw();
    }
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    PrintText(0, 0, "Movable Camera", 0, 1, 0);

    // Night mode sets
    if (nightMode) {
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glEnable(GL_LIGHT2);
    } else {
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glDisable(GL_LIGHT2);
    }

    for (int i = 0; i < 4; i++) {
        if (i == 0) {
            glViewport(0, 0, win.getWidth(), win.getHeight() - 100);
            glMatrixMode(GL_PROJECTION);

            if (currentCamera == 1) {
                glLoadIdentity();
                // gluLookAt(hx, hy, hz, hx + 100 * cos((stuff->bot->getTheta() + stuff->bot->getThetaArm() + 90)*M_PI/180),
                //     hy+100*sin((stuff->bot->getTheta() + stuff->bot->getThetaArm()+ 90)*M_PI/180),
                //     hz+100*cos((stuff->bot->getThetaArmZ() - 90)*M_PI/180),0,0,1);
            } else if (currentCamera == 2) {
                glLoadIdentity();
                gluPerspective(45, win.getWidth() / win.getHeight(), 2, 2000);
                glMatrixMode(GL_MODELVIEW);
                gluLookAt(stuff->bot->getX() + (camDist) * cos((camZangle-90) * M_PI/180)*cos((stuff->bot->getTheta() - 90 + camXYangle) * M_PI/180),
                    stuff->bot->getY() +(camDist*cos(camZangle*M_PI/180)) * sin((stuff->bot->getTheta()-90 + camXYangle) * M_PI/180),
                    camDist*cos((camZangle - 90) * M_PI/180), stuff->bot->getX(), stuff->bot->getY(), 20, 0, 0, 1);
            }
        }
        else if (i == 1) {
            glLoadIdentity();
            glViewport(0, 500, win.getWidth(), 200);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(45, win.getWidth() / 200, 2, 2000);
            glMatrixMode(GL_MODELVIEW);
            gluLookAt(
                stuff->bot->getX(),
                stuff->bot->getY(), 50, 
                stuff->bot->getX() + cos((stuff->bot->getTheta() + 90) * M_PI / 180.0),
                stuff->bot->getY() + sin((stuff->bot->getTheta() + 90) * M_PI / 180.0), 50, 0, 0, 1);

        } else if (i == 2) {
            // minimapa
            // glDisable(GL_LIGHTING);
            glLoadIdentity();
            glViewport(440, 10, win.getWidth()/4, win.getHeight()/4);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(stuff->arena->getX() - stuff->arena->getRadius(), stuff->arena->getX() + stuff->arena->getRadius(),
                stuff->arena->getY() - stuff->arena->getRadius(), stuff->arena->getY() + stuff->arena->getRadius(), -1, 1);
        } else if (i == 3) {
            // minimapa
            glDisable(GL_LIGHTING);
            glLoadIdentity();
            glViewport(0, 0, win.getWidth()/4, win.getHeight()/4);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(stuff->arena->getX() - stuff->arena->getRadius(), stuff->arena->getX() + stuff->arena->getRadius(),
                stuff->arena->getY() - stuff->arena->getRadius(), stuff->arena->getY() + stuff->arena->getRadius(), -1, 1);
            
            renderScoreText();
            glEnable(GL_LIGHTING);
        }

        render(i);

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
    
        // renderScoreText();

        if (i == 1 || i == 0) {
            GLfloat light_position0[] = { stuff->arena->getX()+stuff->center->getRadius()+((stuff->arena->getRadius() - stuff->center->getRadius())/2), stuff->arena->getY(), 100, 1.0 };
            glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
            GLfloat light_position1[] = {stuff->arena->getX()-stuff->center->getRadius()-((stuff->arena->getRadius() - stuff->center->getRadius())/2), stuff->arena->getY(), 100, 1.0 };
            glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

            GLfloat deslocamento = 1;
            GLfloat spotdirectionX = deslocamento*cos(((stuff->bot->getThetaArm()+stuff->bot->getTheta()+90))*M_PI/180);
            GLfloat spotdirectionY = deslocamento*sin(((stuff->bot->getThetaArm()+stuff->bot->getTheta()+90))*M_PI/180);
            GLfloat spotdirectionZ = deslocamento*sin((stuff->bot->getThetaArmZ())*M_PI/180);


            GLfloat spotdirection[] = {spotdirectionX, spotdirectionY, spotdirectionZ};

            GLfloat light_position2[] = {0, 0, 0, 1};
            glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
            glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotdirection);
            glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 20.0);

            //Desenha as luzes
            GLfloat materialEmission[] = { 1, 1, 1, 1.0};
            GLfloat materialColor[] = { 1.0, 1.0, 1.0, 1.0};
            GLfloat materialColorDark[] = { 0.2, 0.2, 0.2, 1.0};
            GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1};
            GLfloat mat_shininess[] = { 128.0 };
            GLfloat ambient[] = { 1,1,1, 1.0 };
            GLfloat noambient[] = { 0,0,0, 1.0 };

            if (nightMode) {
                glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColorDark);
                glMaterialfv(GL_FRONT, GL_AMBIENT, noambient);
            } else {
                glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
                glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
                glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
            }

            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        }
        
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

    GLfloat alturaObstaculo = app->FirstChildElement("obstaculo")->DoubleAttribute("altura") / 100;

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
            stuff->bot = new Robot(cx, -cy, 0, radius);
            stuff->bot->setId(id);
            stuff->bot->setVelocity(vel);
            stuff->bot->setBulletVelocity(velTiro);
            stuff->bot->setStuff(stuff);
            stuff->bot->setHeight(radius * 2.5); // altura do robo
            stuff->bot->setRGB(0, 1, 0);

        } else if (fill == "blue") {
            stuff->arena = new Circle(cx, -cy, 0, radius, 4 * ALT_JOGADOR);
            stuff->arena->setRGB(0, 0, 1);
            stuff->arena->setId(id);
            stuff->arena->setStuff(stuff);

            win.setWidth((GLint) 2 * radius);
            win.setHeight((GLint) 2 * radius);
            win.setTitle("Arena");

        } else if (fill == "white") {
            stuff->center = new Circle(cx, -cy, 0, radius, 59);
            stuff->center->setRGB(0.5, 0.5, 0.5);
            stuff->center->setId(id);
            stuff->center->setStuff(stuff);

        } else if (fill == "red") {
            Enemy* temp = new Enemy(cx, -cy, 0, radius);
            temp->setVelocity(velInimigo);
            temp->setBulletVelocity(velTiroInimigo);
            temp->setId(id);
            temp->setStuff(stuff);
            temp->freqTiro = freqTiro;
            temp->setRGB(1, 0, 0);

            stuff->enemies.push_back(temp);

            // count enemies
            stuff->totalEnemies++;

        } else if (fill == "black") {
            Circle* temp = new Circle(cx, -cy, 0, radius, alturaObstaculo * ALT_JOGADOR);
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
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(win.getWidth(), win.getHeight());
    glutInitWindowPosition (100, 100);

    glutCreateWindow(win.getTitle().c_str());

    // Textures loader (into global objects)
    glEnable(GL_TEXTURE_2D);
    stuff->floorTexture = LoadTexture("textures/floor.bmp");
    stuff->wallsTexture = LoadTexture("textures/walls.bmp");
    stuff->obstaclesTexture = LoadTexture("textures/obstacles.bmp");

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
