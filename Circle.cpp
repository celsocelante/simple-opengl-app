#include "Circle.h"
#include <GLUT/glut.h>
#include <math.h>

Circle::Circle(double c_x, double c_y, double c_z, double c_r) {
    setCoord(c_x, c_y, c_z);
    displayed = false;
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
    
    
}