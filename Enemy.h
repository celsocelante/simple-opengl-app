#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "Robot.h"

#ifndef ENEMY_H
#define ENEMY_H

class Enemy: public Robot {
    public:
        Enemy(GLfloat x, GLfloat y, GLfloat z, GLfloat radius) : Robot(x, y, z, radius) {
            this->red = 1;
            this->green = 0;
            this->blue = 0;

            this->type = 3;
        }
        void update();
        GLfloat freqTiro = 1;
        GLfloat lastTimeShot = 0;
};
#endif