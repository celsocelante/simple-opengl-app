#include "Circle.h"

Circle::Circle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLfloat height) {
    setCoord(x, y, z);
    this->beingDragged = false;
    this->beingResized = false;
    this->jumping = false;
    this->radius = radius;
    this->height = height;
    this->previousRadius = radius;

    this->red = this->green = this->blue = 0;
    this->id = -1;
    this->type = 1;
}

Circle::Circle() {
    this->beingDragged = false;
    this->beingResized = false;
    this->x = this->y = this->z = 0;
    this->jumping = false;
    this->radius = 0;
    this->previousRadius = this->radius;

    this->red = this->green = this->blue = 0;
    this->id = -1;
}

void Circle::setX(GLfloat x) {
    this->x = x;
}

void Circle::setY(GLfloat y) {
    this->y = y;
}
void Circle::setZ(GLfloat z) {
    this->z = z;
}
void Circle::setCoord(GLfloat x, GLfloat y, GLfloat z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void Circle::setRadius(GLfloat radius) {
    this->radius = radius;
}

void Circle::setDisplayed(bool display) {
    this->displayed = display;
}


void Circle::draw() {
    GLfloat twicePi = 2.0f * M_PI;
    GLint triangles = 100;
    GLint i;
    GLfloat angle_stepsize = 0.1;
    GLfloat angle = 0;
    
    /** Draw the tube */
    glColor3f(1 - red, 1 - green, 1 - blue);
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
    for(GLint i = 0; i <= triangles; i ++) {
        glVertex3f(
            x + (radius * cos(i *  twicePi / triangles)),
            y + (radius * sin(i * twicePi / triangles)),
        0
        );
        glVertex3f(
            x + (radius * cos(i *  twicePi / triangles)),
            y + (radius * sin(i * twicePi / triangles)),
        height
        );
    }
    glVertex3f(x + radius, y, height);
    glVertex3f(x + radius, y, 0.0);
    glEnd();

    /** Draw the circle on top of cylinder */
    glColor3f(red, green, blue);
    glBegin(GL_POLYGON);
    angle = 0.0;
    for(GLint i = 0; i <= triangles; i ++) {
        glVertex3f(
            x + (radius * cos(i *  twicePi / triangles)),
            y + (radius * sin(i * twicePi / triangles)),
        height
        );
    }
    glVertex3f(radius, 0.0, height);
    glEnd();
    glBegin(GL_POLYGON);
    
    angle = 0.0;
    for(GLint i = 0; i <= triangles; i ++) {
        glVertex3f(
            x + (radius * cos(i *  twicePi / triangles)),
            y + (radius * sin(i * twicePi / triangles)),
        0.0
        );
    }

    glVertex3f(radius, 0.0, 0.0);
    glEnd();



    glLineWidth(0.5);
    glBegin(GL_LINE_LOOP);
    glColor3f(0.5, 0.5, 0.5);
		glVertex3f(x, y, -5); // center of circle
		for(GLint i = 0; i <= triangles; i ++) { 
			glVertex3f(
		        x + (radius * cos(i *  twicePi / triangles)), 
			    y + (radius * sin(i * twicePi / triangles)), 0
			);
		}
	glEnd();
}


void Circle::setRGB(GLfloat red, GLfloat green, GLfloat blue) {
    this->red = red;
    this->green = green;
    this->blue = blue;
}

void Circle::setDragState(bool bgd) {
    this->beingDragged = bgd;
}

void Circle::setResizeState(bool rs) {
    this->beingResized = rs;
}

void Circle::move(GLfloat x, GLfloat y, GLfloat z) {
    this->x += x;
    this->y += y;
    this->z += z;
}

bool Circle::collision(Circle *c, GLfloat dx, GLfloat dy) {
    GLfloat temp = sqrt(pow(c->getX() - (this->x + dx), 2) +
                            pow(c->getY() - (this->y + dy), 2));

	return temp <= (c->getRadius() + this->radius);
}

bool Circle::collisionNoDist(GLfloat x, GLfloat y, GLfloat radius) {
    GLfloat temp = sqrt(pow(x - this->x, 2) +
                            pow(y - this->y, 2));

	return temp <= (radius + this->radius);
}

void Circle::setJumping(bool value) {
    this->jumping = value;
}

void Circle::setId(GLint id) {
    this->id = id;
}

void Circle::changeRadius(GLfloat i) {
    this->radius += i;

    if (this->radius < 0) {
        this->radius *= -1;
    }
}

void Circle::restoreRadius() {
    this->radius = this->previousRadius;
}

void Circle::setStuff(Stuff* stuff) {
    this->stuff = stuff;
}
