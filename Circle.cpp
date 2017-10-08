#include "Circle.h"

Circle::Circle(double c_x, double c_y, double c_z, double c_r) {
    setCoord(c_x, c_y, c_z);
    displayed = true;
    beingDragged = false;
    beingResized = false;
    jumping = false;
    radius = c_r;
    previousRadius = radius;

    red = green = blue = 0;
    id = -1;
}

Circle::Circle() {
    displayed = false;
    beingDragged = false;
    beingResized = false;
    x = y = z = 0;
    jumping = false;
    radius = 0;
    previousRadius = radius;

    red = green = blue = 0;
    id = -1;
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
    GLfloat twicePi = 2.0f * M_PI;
    int triangles = 3000;
	
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

void Circle::move(double x, double y, double z) {
    this->x += x;
    this->y += y;
    this->z += z;
}

bool Circle::collision(Circle *c, double dx, double dy) {
    double temp = sqrt(pow(c->getX() - (x + dx), 2) + 
                            pow(c->getY() - (y + dy), 2));

	return temp <= (c->getRadius() + radius);
}

void Circle::setJumping(bool value) {
    jumping = value;
}

void Circle::setId(int id) {
    this->id = id;
}

void Circle::changeRadius(double i) {
    radius += i;

    if (radius < 0) {
        radius *= -1;
    }
}

void Circle::restoreRadius() {
    radius = previousRadius;
}