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

#ifndef ENEMY_H
#define ENEMY_H

class Enemy: public Robot {
    public:
        Enemy(GLfloat x, GLfloat y, GLfloat z, GLfloat radius) : Robot(x, y, z, radius) {
            this->red = 1;
            this->green = 0;
            this->blue = 0;
        }
};
#endif