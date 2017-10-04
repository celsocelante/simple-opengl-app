#include "Circle.h"

Circle::Circle(double c_x, double c_y, double c_z, double c_r) {
    setCoord(c_x, c_y, c_z);
    displayed = false;
    beingDragged = false;
    beingResized = false;
    radius = c_r;

    red = green = blue = 0;
}

Circle::Circle() {}

void Circle::setX(double c_x) {
    x = c_x;
}

void Circle::setY(double c_y) {
    y = c_y;
}
void Circle::setZ(double c_z) {
    z = c_z;
}
void Circle::setCoord(double c_x, double c_y, double c_z) {
    x = c_x;
    y = c_y;
    z = c_z;
}

void Circle::setRadius(double c_r) {
    radius = c_r;
}

void Circle::setDisplayed(bool c_display) {
    displayed = c_display;
}


void Circle::draw() {
    int i;
    
        glColor3dv(red, green, blue);
        glBegin(GL_TRIANGLE_FAN);
            glVertex3d(x, y, z);
            
            for(i = 0; i <= 1000; ++i) {
                glVertex3d(
                    x + (radius * cos(i * M_PI / 1000)), 
                    y + (radius * sin(i * M_PI / 1000)),
                    z
                );
            }
        glEnd();
}

void Circle::setRGB(double r, double g, double b) {
    red = r;
    green = g;
    blue = b;
}

void Circle::setDragState(bool bgd) {
    beingDragged = bgd;
}

void Circle::setResizeState(bool rs) {
    beingResized = rs;
}