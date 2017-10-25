#include "Robot.h"

Robot::Robot() {}

Robot::Robot(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void Robot::drawRectangle(double width, double height, double R, double G, double B) {
    glColor3f(R, G, B);
    
    glBegin(GL_QUADS);
        glVertex2f( -width/2.0, 0.0);
        glVertex2f( -width/2.0, height);
        glVertex2f( width/2.0, height);
        glVertex2f( width/2.0, 0.0);
    glEnd();
}

void Robot::drawCircle(double radius, double R, double G, double B) {
    GLfloat twicePi = 2.0f * M_PI;
    int triangles = 3000;

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(R, G, B);
		glVertex3f(x, y, 0); // center of circle
		for(int i = 0; i <= triangles; i ++) { 
			glVertex3f(
		            x + (radius * cos(i *  twicePi / triangles)), 
			    y + (radius * sin(i * twicePi / triangles)), 0
			);
		}
	glEnd();
}

void Robot::drawEllipse() {

}

void Robot::rotateRight() {

}

void Robot::rotateLeft() {

}

void Robot::moveForward() {

}

void Robot::moveBackward() {

}

void Robot::draw() {

}