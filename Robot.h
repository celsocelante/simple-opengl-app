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
using namespace std;

#ifndef ROBOT_H
#define ROBOT_H

class Robot {
    private:
        double x, y, z;
        double radius = 20; // Raio do circulo virtual em torno do robo 
        double thetaArm = 0;
        double theta = 0;
        double i;

        void drawRectangle(double width, double height, double R, double G, double B);
        void drawCircle(double radius, double R, double G, double B);
        void drawEllipse(double xRadius, double YRadius);

    public:
        Robot();
        Robot(double x, double y, double z);
        void rotateRight();
        void rotateLeft();
        void moveForward();
        void moveBackward();
        void draw();

};

#endif