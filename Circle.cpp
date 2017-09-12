#include "Circle.h"

Circle:Circle(int c_x, int c_y, int c_z) {
    setCoord(c_x, c_y, c_z);
    displayed = false;
}

void Circle:setX(int c_x) {
    x = c_x;
}

void Circle:setY(int c_y) {
    y = c_y;
}
void Circle:setZ(int c_z) {
    z = c_z;
}
void Circle:setCoord(int c_x, int c_y, int c_z) {
    x = c_x;
    y = c_y;
    z = c_z;
}

void Circle:setDisplayed(bool c_display) {
    displayed = c_display;
}