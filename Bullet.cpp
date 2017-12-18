#include "Bullet.h"

Bullet::Bullet(GLfloat x, GLfloat y, GLfloat z, GLfloat theta, GLfloat thetaRobot, GLfloat thetaZ, GLfloat vel, GLfloat radius) {
	this->x = x;
	this->y = y;
    this->z = z;
	this->theta = theta;
    this->thetaRobot = thetaRobot;
    this->thetaZ = thetaZ;
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

void Bullet::setZ(GLfloat z){
	this->z = z;
}

void Bullet::update(GLfloat time) {
    if (this->displayed) {
        GLfloat dir = this->thetaRobot + this->theta;

        this->x = this->x + time * this->vel * cos((dir + 90) * M_PI/180) * sin((90 - thetaZ) * M_PI/180);
        this->y = this->y + time * this->vel * sin((dir + 90) * M_PI/180) * sin((90 - thetaZ) * M_PI/180);  
        this->z = this->z + time * this->vel * cos((90 - thetaZ) * M_PI/180);  
    }  
}

bool Bullet::collision(Circle *c, GLfloat dx, GLfloat dy, GLfloat dz) {
    GLfloat temp = sqrt(pow(c->getX() - (this->x + dx), 2) + 
                            pow(c->getY() - (this->y + dy), 2) +
                            pow(c->getZ() - (this->z + dz), 2));

	return temp <= (c->getRadius() + this->radius);
}

void Bullet::draw(){
    if (this->displayed) {
        glPushMatrix();
            glColor3f(1, 1, 0); // yellow

            GLfloat materialColorD[] = { 1, 1, 0, 1};
            GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1};
            GLfloat mat_shininess[] = { 70.0 };
            GLfloat ambient[] = { 0.2, 0.2, 0.2, 1};

            glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColorD);
            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

            glTranslatef(this->x, this->y, this->z);
            // glRotatef(this->thetaRobot, 0, 0, 1);
            // glTranslatef(-(this->radius - 0.6), 0, 0);
            // glRotatef(this->theta, 0, 0, 1);
            // glTranslatef(0, this->radius, 0);
            // drawCircle(0, 0, SIZE_BULLET);
            glutSolidSphere(SIZE_BULLET, 100, 100);

        glPopMatrix();
    }
}