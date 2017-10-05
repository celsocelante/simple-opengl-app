#include "Circle.h"

Circle::Circle(double c_x, double c_y, double c_z, double c_r) {
    setCoord(c_x, c_y, c_z);
    displayed = false;
    beingDragged = false;
    beingResized = false;
    jumping = false;
    flying = false;
    radius = c_r;
    previousRadius = radius;

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
    GLfloat twicePi = 2.0f * M_PI;
    int triangles = 1000;
	
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(red, green, blue);
		glVertex3f(x, y, 0); // center of circle
		for(int i = 0; i <= triangles; i ++) { 
			glVertex3f(
		            x + (radius * cos(i *  twicePi / triangles)), 
			    y + (radius * sin(i * twicePi / triangles)), 0
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

void Circle::move(double x, double y) {
    this->x += x;
    this->y += y;
}

bool Circle::collision(Circle *c) {
    double temp = sqrt(pow(c->getX() - x, 2) + 
                            pow(c->getY() - y, 2));

	return temp <= (c->getRadius() + radius);
}

bool Circle::collision(double x, double y, double r) {
	double temp = sqrt(pow(this->x - x, 2) + 
                            pow(this->y - y, 2));

    return temp <= (this->radius + r);
}