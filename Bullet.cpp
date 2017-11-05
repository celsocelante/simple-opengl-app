#include "Bullet.h"
#define SIZE 3.0

Bullet::Bullet(GLfloat x, GLfloat y, GLfloat theta, GLfloat thetaRobot, GLfloat vel, GLfloat armLength, GLfloat width) {
	// this->x = x;
	// this->y = y;
	this->theta = theta;
    this->thetaRobot = thetaRobot;
	this->vel = vel;
    this->armLength = armLength;
    this->radius = SIZE;

    this->x = (x - width) * cos((theta - 90) * M_PI / 180) * vel;
    this->y = (y + armLength) * sin((theta - 90) * M_PI / 180) * vel;

    cout << this->x << " " << this->y << endl;
}

void Bullet::drawCircle() {
    GLfloat twicePi = 2.0f * M_PI;
    GLint triangles = 100;

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1, 1, 0);
		glVertex3f(this->x, this->y, 0);
		for(GLint i = 0; i <= triangles; i ++) { 
			glVertex3f(
                this->x + (radius * cos(i *  twicePi / triangles)), 
			    this->y + (radius * sin(i * twicePi / triangles)), 0
			);
		}
	glEnd();
}

GLfloat Bullet::getAngle(){
	return this->theta;
}

GLfloat Bullet::getX(){
    return this->x;
}

GLfloat Bullet::getY(){
    return this->y;
}

void Bullet::setX(GLfloat x){
	this->x = x;
}

void Bullet::setY(GLfloat y){
	this->y = y;
}

GLfloat Bullet::getRadius(){
	return radius;
}

GLfloat Bullet::getVel(){
	return this->vel;
}

GLfloat Bullet::getRobotAngle(){
	return this->thetaRobot;
}

void Bullet::update() {
    this->x = x + cos((this->theta - 90) * M_PI / 180) * this->vel;
    this->y = y + sin((this->theta - 90) * M_PI / 180) * this->vel;
}


void Bullet::draw(){
    drawCircle();
    update();
}