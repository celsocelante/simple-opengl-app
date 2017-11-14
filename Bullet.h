#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif

#include <math.h>
#include <iostream>
#include <string>
#include <list>
#include "Circle.h"
using namespace std;
#define SIZE_BULLET 3.0

#ifndef BULLET_H
#define BULLET_H

class Bullet {
    private:
        GLint x;
        GLint y;
        GLfloat theta;
        GLfloat thetaRobot;
        GLfloat vel;
        GLfloat radius;
        bool enabled = true;
        void drawCircle(GLfloat x, GLfloat y, GLfloat radius);

    public:
        Bullet(GLfloat x, GLfloat y, GLfloat theta, GLfloat thetaRobot, GLfloat vel, GLfloat radius);
        GLfloat getTheta() { return this->theta; }
        GLfloat getX() { return this->x; }
        GLfloat getY() { return this->y; }
        GLfloat getRadius() { return this->radius; }
        GLfloat getVel() { return this->vel; }
        GLfloat getThetaRobot() { return this->thetaRobot; }
        GLfloat getVelocity() { return this->vel; }
        GLfloat getEnabled() { return this->enabled; }
        void setX(GLfloat x);
        void setY(GLfloat y);
        void setEnabled(bool value);
        void draw();
        void update(GLfloat time);
        bool collision(Circle *c, GLfloat dx, GLfloat dy);
};

#endif