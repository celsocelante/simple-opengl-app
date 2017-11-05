#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <iostream>
#include <string>
#include <list>
using namespace std;

#ifndef BULLET_H
#define BULLET_H

class Bullet {
    private:
        GLint x;
        GLint y;
        GLfloat theta;
        GLfloat thetaRobot;
        GLfloat vel;
        GLfloat armLength;
        GLfloat radius;
        void drawCircle();

    public:
        Bullet(GLfloat x, GLfloat y, GLfloat theta, GLfloat thetaRobot, GLfloat vel, GLfloat armLength, GLfloat width);
        GLfloat getAngle();
        GLfloat getX();
        GLfloat getY();
        void setX(GLfloat x);
        void setY(GLfloat y);
        GLfloat getRadius();
        GLfloat getVel();
        GLfloat getRobotAngle();
        void draw();
        void update();
};

#endif