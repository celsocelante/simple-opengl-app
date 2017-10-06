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

#ifndef CIRCLE_H
#define CIRCLE_H

class Circle {
    private:
        double x, y, z;
        double radius;
        double previousRadius;

        double px, py, pz;
        double pradius;
        
        bool displayed;
        bool beingDragged;
        bool beingResized;

        bool jumping, flying;

        double red, green, blue;
    
    public:
        Circle(double c_x, double c_y, double c_z, double c_r);
        Circle();

        void setX(double c_x);
        void setY(double c_y);
        void setZ(double c_z);
        void setCoord(double c_x, double c_y, double c_z);
        void setRadius(double c_r);
        void setDisplayed(bool d);
        void setDragState(bool bgd);
        void setResizeState(bool rs);
        void setRGB(double r, double g, double b);
        void draw();

        double getX() { return x; }
        double getY() { return y; }
        double getZ() { return z; }
        double getRadius() { return radius; }
        bool getDisplayed() { return displayed; }
        bool getDragState() { return beingDragged; }
        bool getResizeState() { return beingResized; }
        void move(double x, double y, double z);
        bool collision(Circle *c, double dx, double dy);
        bool collision(double x, double y, double radius);
        bool inside(Circle *c, double dx, double dy);
        void setJumping(bool value);
        bool getJumping();

};
#endif