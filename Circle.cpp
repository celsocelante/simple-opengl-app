#include "Circle.h"

Circle::Circle(double c_x, double c_y, double c_z, double c_r) {
    setCoord(c_x, c_y, c_z);
    displayed = false;
    beingDragged = false;
    beingResized = false;
    radius = c_r;
}

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
    glClear(GL_COLOR_BUFFER_BIT);
    GLfloat twicePi = 2.0f * M_PI;

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(0.001);
    
    glBegin(GL_LINES);
        glColor4f(1.0, 1, 1, 1.0);

        for(int i = 0; i <= 1000; i++) {
            glVertex3f(x, y, 0);
            glVertex3f(x + (radius * cos(i * twicePi / 1000)),
                y + (radius * sin(i * twicePi / 1000)), 0);
        }
    glEnd();
}

void Circle::setDragState(bool bgd) {
    beingDragged = bgd;
}

void Circle::setResizeState(bool rs) {
    beingResized = rs;
}