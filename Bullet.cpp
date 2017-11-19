#include "Bullet.h"

Bullet::Bullet(GLfloat x, GLfloat y, GLfloat theta, GLfloat thetaRobot, GLfloat vel, GLfloat radius) {
	this->x = x;
	this->y = y;
	this->theta = theta;
    this->thetaRobot = thetaRobot;
	this->vel = vel;
    this->radius = radius;
    this->displayed = true;
}

void Bullet::drawCircle(GLfloat x, GLfloat y, GLfloat radius) {
    GLfloat twicePi = 2.0f * M_PI;
    GLint triangles = 40;

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1, 1, 0); // yellow
		glVertex3f(x, y, 0);
		for(GLint i = 0; i <= triangles; i ++) { 
			glVertex3f(
                x + (radius * cos(i *  twicePi / triangles)), 
			    y + (radius * sin(i * twicePi / triangles)), 0
			);
		}
	glEnd();
}

void Bullet::setX(GLfloat x){
	this->x = x;
}

void Bullet::setY(GLfloat y){
	this->y = y;
}

void Bullet::update(GLfloat time) {
    if (this->displayed) {
        GLfloat dir = this->thetaRobot - this->theta;
        
        this->x = this->x + time * this->vel * cos((dir + 90) * M_PI/180);
        this->y = this->y + time * this->vel * sin((dir + 90) * M_PI/180);  
    }  
}

bool Bullet::collision(Circle *c, GLfloat dx, GLfloat dy) {
    GLfloat temp = sqrt(pow(c->getX() - (this->x + dx), 2) + 
                            pow(c->getY() - (this->y + dy), 2));

	return temp <= (c->getRadius() + this->radius);
}

void Bullet::draw(){
    if (this->displayed) {
        glPushMatrix();
            glTranslatef(this->x, this->y, 0);
            glRotatef(this->thetaRobot, 0, 0, 1);
            glTranslatef(-(this->radius - 0.6), 0, 0);
            glRotatef(-this->theta, 0, 0, 1);
            glTranslatef(0, this->radius, 0);
            drawCircle(0, 0, SIZE_BULLET);
        glPopMatrix();
    }
}