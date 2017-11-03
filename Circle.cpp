#include "Circle.h"

Circle::Circle(GLfloat c_x, GLfloat c_y, GLfloat c_z, GLfloat c_r) {
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

void Circle::setX(GLfloat c_x) {
    x = c_x;
}

void Circle::setY(GLfloat c_y) {
    y = c_y;
}
void Circle::setZ(GLfloat c_z) {
    z = c_z;
}
void Circle::setCoord(GLfloat c_x, GLfloat c_y, GLfloat c_z) {
    x = c_x;
    y = c_y;
    z = c_z;
}

void Circle::setRadius(GLfloat c_r) {
    radius = c_r;
}

void Circle::setDisplayed(bool c_display) {
    displayed = c_display;
}


void Circle::draw() {
    GLfloat twicePi = 2.0f * M_PI;
    GLint triangles = 3000;
	
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(red, green, blue);
		glVertex3f(x, y, 0); // center of circle
		for(GLint i = 0; i <= triangles; i ++) { 
			glVertex3f(
		            x + (radius * cos(i *  twicePi / triangles)), 
			    y + (radius * sin(i * twicePi / triangles)), 0
			);
		}
	glEnd();
}

void Circle::setRGB(GLfloat r, GLfloat g, GLfloat b) {
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

void Circle::move(GLfloat x, GLfloat y, GLfloat z) {
    this->x += x;
    this->y += y;
    this->z += z;
}

bool Circle::collision(Circle *c, GLfloat dx, GLfloat dy) {
    GLfloat temp = sqrt(pow(c->getX() - (x + dx), 2) + 
                            pow(c->getY() - (y + dy), 2));

	return temp <= (c->getRadius() + radius);
}

void Circle::setJumping(bool value) {
    jumping = value;
}

void Circle::setId(GLint id) {
    this->id = id;
}

void Circle::changeRadius(GLfloat i) {
    radius += i;

    if (radius < 0) {
        radius *= -1;
    }
}

void Circle::restoreRadius() {
    radius = previousRadius;
}