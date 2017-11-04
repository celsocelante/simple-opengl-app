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
#include "Bullet.h"
#include "Circle.h"
using namespace std;

#ifndef ROBOT_H
#define ROBOT_H

class Robot: public Circle {
    private:
        GLfloat x, y, z;
        GLfloat radius = 20; // Raio do circulo virtual em torno do robo 
        GLfloat thetaArm = 0;
        GLfloat theta = 0;
        GLfloat scale;

        void drawRectangle(GLfloat width, GLfloat height, GLfloat R, GLfloat G, GLfloat B);
        void drawRectangleLine(GLfloat width, GLfloat height, GLfloat R, GLfloat G, GLfloat B);
        void drawCircle(GLfloat radius, GLfloat R, GLfloat G, GLfloat B);
        void drawCircleLine(GLfloat radius, GLfloat R, GLfloat G, GLfloat B);
        void drawEllipse(GLfloat xRadius, GLfloat YRadius, GLfloat R, GLfloat G, GLfloat B);
        void drawEllipseLine(GLfloat xRadius, GLfloat YRadius, GLfloat R, GLfloat G, GLfloat B);

    public:
        Robot();
        Robot(GLfloat x, GLfloat y, GLfloat z);
        void rotateRight();
        void rotateLeft();
        void rotateArmRight();
        void rotateArmLeft();
        void moveForward();
        void moveBackward();
        void shoot(list<Bullet>& bullets);
        void draw();

        GLfloat getScale() { return this->scale; }
        void setScale(GLfloat scale);
        void changeScale(GLfloat i);

};

#endif