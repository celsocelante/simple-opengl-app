#include <GLUT/glut.h>
#include <math.h>

#ifndef CIRCLE_H
#define CIRCLE_H

class Circle {
    private:
        double x, y, z;
        double radius;

        double px, py, pz;
        double pradius;
        
        bool displayed;
        bool beingDragged;
        bool beingResized;

        double red, green, blue;
    
    public:
        Circle(double c_x, double c_y, double c_z, double c_r);

        void setX(double c_x);
        void setY(double c_y);
        void setZ(double c_z);
        void setCoord(double c_x, double c_y, double c_z);
        void setRadius(double c_r);
        void setDisplayed(bool d);
        void setDragState(bool bgd);
        void setResizeState(bool rs);
        void draw();

        double getX() { return x; }
        double getY() { return y; }
        double getZ() { return z; }
        double getRadius() { return radius; }
        bool getDisplayed() { return displayed; }
        bool getDragState() { return beingDragged; }
        bool getResizeState() { return beingResized; }
};
#endif