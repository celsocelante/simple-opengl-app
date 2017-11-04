#include "Bullet.h"


Bullet::Bullet(GLfloat x, GLfloat y, GLfloat theta, GLfloat thetaRobot, GLfloat vel, GLfloat armLength) {
	this->x = x;
	this->y = y;
	this->theta = theta;
    this->thetaRobot = thetaRobot;
	this->vel = vel;
    this->armLength = armLength;
}

void Bullet::drawCircle(GLfloat radius, GLfloat R, GLfloat G, GLfloat B) {
    GLfloat twicePi = 2.0f * M_PI;
    GLint triangles = 10;

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(R, G, B);
		glVertex3f(0, 0, 0);
		for(GLint i = 0; i <= triangles; i ++) { 
			glVertex3f(
                0 + (radius * cos(i *  twicePi / triangles)), 
			    0 + (radius * sin(i * twicePi / triangles)), 0
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
	return 4.0;
}

GLfloat Bullet::getVel(){
	return this->vel;
}

GLfloat Bullet::getRobotAngle(){
	return this->thetaRobot;
}

void Bullet::update() {
    this->x += 2;
    this->y += 2;
}


void Bullet::draw(){
	glPushMatrix();
		glTranslatef(this->x, this->y, 0);
		drawCircle(4, 1, 0, 0);
    glPopMatrix();
    update();

    x = x + 1;
    y = y + 1;

    cout << this->x << " " << this->y << endl;
}