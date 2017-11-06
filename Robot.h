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

#define ARM_MOVEMENT 3
#define ARM_ANGLE 45
#define MOVEMENT 5
#define SWAP_LEGS_COUNT 20

#ifndef ROBOT_H
#define ROBOT_H

class Robot: public Circle {
    private:
        GLfloat thetaArm = 0;
        GLfloat theta = 0;
        GLfloat scale;
        GLfloat velocity;
        GLfloat bulletVelocity;
        GLfloat stepsCounter = 0;
        GLfloat previousScale = 1;
        bool legs;

        void drawRectangle(GLfloat width, GLfloat height, GLfloat R, GLfloat G, GLfloat B);
        void drawRectangleLine(GLfloat width, GLfloat height, GLfloat R, GLfloat G, GLfloat B);
        void drawCircle(GLfloat radius, GLfloat R, GLfloat G, GLfloat B);
        void drawCircleLine(GLfloat radius, GLfloat R, GLfloat G, GLfloat B);
        void drawEllipse(GLfloat xRadius, GLfloat YRadius, GLfloat R, GLfloat G, GLfloat B);
        void drawEllipseLine(GLfloat xRadius, GLfloat YRadius, GLfloat R, GLfloat G, GLfloat B);

    public:
        Robot();
        Robot(GLfloat x, GLfloat y, GLfloat z, GLfloat radius);
        void rotateRight();
        void rotateLeft();
        void rotateArmRight();
        void rotateArmLeft();
        void moveForward();
        void moveBackward();
        void shoot(list<Bullet>& bullets);
        void draw();
        // bool collision(Circle *c, GLfloat dx, GLfloat dy);

        GLfloat getScale() { return this->scale; }
        GLfloat getTheta() { return this->theta; }
        void setScale(GLfloat scale);
        void changeScale(GLfloat i);
        void restoreScale();
        void setVelocity(GLfloat velocity);
        void setBulletVelocity(GLfloat bulletVelocity);
        GLfloat getVelocity() { return this->velocity; }
        GLfloat getBulletVelocity() { return this->bulletVelocity; }
        void swapLegs();

};

#endif