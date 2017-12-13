#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include <math.h>
#include <iostream>
#include <string>
#include <list>
#include <random>
#include "Bullet.h"
#include "Circle.h"
using namespace std;

#define ARM_MOVEMENT 3
#define ARM_ANGLE 45
#define ROTATE 2
#define MOVEMENT 5
#define SWAP_LEGS_COUNT 20
#define ANIMATION_FRAMES 30
#define ANIMATION_TIME 2000
#define FACTOR 0.5

#ifndef ROBOT_H
#define ROBOT_H

class Robot: public Circle {
    protected:
        GLfloat thetaArm = 0;
        GLfloat theta = 180;
        GLfloat scale;
        GLfloat velocity = 1;
        GLfloat bulletVelocity;
        GLfloat stepsCounter = 0;
        GLfloat previousScale = 1;
        GLfloat height = 80;
        bool moveFreely = false;
        bool legs;
        list<Bullet> bullets;
        GLfloat red, green, blue;
        Circle disabledLowObstacle;
        
        

        void drawRectangle(GLfloat width, GLfloat height, GLfloat R, GLfloat G, GLfloat B);
        void drawRectangleLine(GLfloat width, GLfloat height, GLfloat R, GLfloat G, GLfloat B);
        void drawCircle(GLfloat radius, GLfloat R, GLfloat G, GLfloat B);
        void drawCircleLine(GLfloat radius, GLfloat R, GLfloat G, GLfloat B);
        void drawEllipse(GLfloat xRadius, GLfloat YRadius, GLfloat R, GLfloat G, GLfloat B);
        void drawEllipseLine(GLfloat xRadius, GLfloat YRadius, GLfloat R, GLfloat G, GLfloat B);

        void jumpStart(GLint v);
        void jumpMiddle(GLint v);
        void jumpEnd(GLint v);

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
        bool canMoveFreely() { return this->moveFreely; }
        void setMoveFreely(bool moveFreely);

        GLfloat getScale() { return this->scale; }
        GLfloat getTheta() { return this->theta; }
        GLfloat getThetaArm() { return this->thetaArm; }
        GLfloat getHeight() { return this->height; }
        GLfloat newX();
        GLfloat newY();
        void setScale(GLfloat scale);
        void changeScale(GLfloat i);
        void restoreScale();
        void setVelocity(GLfloat velocity);
        void setBulletVelocity(GLfloat bulletVelocity);
        void setHeight(GLfloat height);
        GLfloat getVelocity() { return this->velocity; }
        GLfloat getBulletVelocity() { return this->bulletVelocity; }
        void swapLegs();
        bool ableToMove(GLfloat dx, GLfloat dy, GLfloat dz);
        void setFire();
        void jump();
        void getGunPositionX();
        void getGunPositionY();  
        void getGunPositionZ();
};

#endif