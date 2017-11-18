#include "Robot.h"

Robot::Robot() {}

Robot::Robot(GLfloat x, GLfloat y, GLfloat z, GLfloat radius) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->radius = radius;
    this->previousRadius = radius;
    this->scale = 1.0;
    this->previousScale = this->scale;
    this->stepsCounter = 0;
    this->legs = true;
    this->moveFreely = false;


    // Colors
    this->red = 0;
    this->green = 1;
    this->blue = 0;
}

void Robot::setMoveFreely(bool moveFreely) {
    this->moveFreely = moveFreely;
}

void Robot::setVelocity(GLfloat velocity) {
    this->velocity = velocity;
}

void Robot::setBulletVelocity(GLfloat bulletVelocity) {
    this->bulletVelocity = bulletVelocity;
}

void Robot::restoreScale() {
    this->scale = this->previousScale;
}

void Robot::drawRectangle(GLfloat width, GLfloat height, GLfloat R, GLfloat G, GLfloat B) {
    glColor3f(R, G, B);
    
    glBegin(GL_QUADS);
        glVertex2f( -width/2.0, 0.0);
        glVertex2f( -width/2.0, height);
        glVertex2f( width/2.0, height);
        glVertex2f( width/2.0, 0.0);
    glEnd();
}

void Robot::drawRectangleLine(GLfloat width, GLfloat height, GLfloat R, GLfloat G, GLfloat B) {
    GLint lineWidth = 2;
    
    glLineWidth(lineWidth);
    glColor3f(R, G, B);
    
    glBegin(GL_LINES);
        glVertex2f( -width/2.0, 0.0);
        glVertex2f( -width/2.0, height);
        glVertex2f( width/2.0, height);
        glVertex2f( width/2.0, 0.0);
    glEnd();
}

void Robot::drawCircle(GLfloat radius, GLfloat R, GLfloat G, GLfloat B) {
    GLfloat twicePi = 2.0f * M_PI;
    GLint triangles = 300;

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(R, G, B);
		glVertex3f(0, 0, 0); // center of circle
		for(GLint i = 0; i <= triangles; i ++) { 
			glVertex3f(
                0 + (radius * cos(i *  twicePi / triangles)), 
			    0 + (radius * sin(i * twicePi / triangles)), 0
			);
		}
	glEnd();
}

void Robot::drawCircleLine(GLfloat radius, GLfloat R, GLfloat G, GLfloat B) {
    GLfloat twicePi = 2.0f * M_PI;
    GLint triangles = 300;
    GLint lineWidth = 2;

    glLineWidth(lineWidth);
    glBegin(GL_LINE_LOOP);
    glColor3f(R, G, B);
		glVertex3f(0, 0, 0); // center of circle
		for(GLint i = 0; i <= triangles; i ++) { 
			glVertex3f(
		        0 + (radius * cos(i *  twicePi / triangles)), 
			    0 + (radius * sin(i * twicePi / triangles)), 0
			);
		}
	glEnd();
}

void Robot::drawEllipse(GLfloat xRadius, GLfloat YRadius, GLfloat R, GLfloat G, GLfloat B) {
    GLint i;

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(R, G, B);
    for (i = 0; i < 360; i++) {
        GLfloat rad = i * (M_PI/180);
        glVertex2f(cos(rad) * xRadius, sin(rad) * YRadius);
    }
    glEnd();
}

void Robot::drawEllipseLine(GLfloat xRadius, GLfloat YRadius, GLfloat R, GLfloat G, GLfloat B) {
    GLint i;
    GLint lineWidth = 2;
    
    glLineWidth(lineWidth);
    glBegin(GL_LINE_LOOP);
    glColor3f(R, G, B);
    for (i = 0; i < 360; i++) {
        GLfloat rad = i * (M_PI/180);
        glVertex2f(cos(rad) * xRadius, sin(rad) * YRadius);
    }
    glEnd();
}

void Robot::rotateArmRight() {
    if (this->thetaArm < ARM_ANGLE) {
        this->thetaArm += ARM_MOVEMENT;
    }
}

void Robot::rotateArmLeft() {
    if (this->thetaArm > -ARM_ANGLE) {
        this->thetaArm -= ARM_MOVEMENT;
    }
}

void Robot::rotateRight() {
    this->theta += 1;
}

void Robot::rotateLeft() {
    this->theta -= 1;
}

void Robot::swapLegs() {
    this->stepsCounter = this->stepsCounter + 1;
    if (this->stepsCounter >= SWAP_LEGS_COUNT) {
        this->legs = !this->legs;

        this->stepsCounter = 0;
    }
}

GLfloat Robot::newX() {
    return cos((this->theta - 90) * M_PI / 180) * this->velocity * MOVEMENT;
}

GLfloat Robot::newY() {
    return sin((this->theta - 90) * M_PI / 180) * this->velocity * MOVEMENT;
}

void Robot::moveForward() {
    swapLegs();
    x -= newX();
    y -= newY();
}

void Robot::moveBackward() {
    swapLegs();
    x += newX();
    y += newY();
}

void Robot::setScale(GLfloat scale) {
    this->scale = scale;
}

void Robot::changeScale(GLfloat i) {
    this->scale += i;
}


void Robot::draw() {
    glPushMatrix();
        glTranslatef(0, 0, 0);
            glPushMatrix();

                glTranslatef(x, y, 0);
                glRotatef(this->theta, 0, 0, 1);

                glPushMatrix();
                    glScaled(this->scale, this->scale, 0);
                glPopMatrix();

                // Perna direita
                if (this->legs) {
                    glPushMatrix();
                        glTranslatef(-this->radius/3, this->radius/5, 0);
                        drawRectangle(this->radius/3, this->radius/2, 0, 0, 0);
                    glPopMatrix();
                }

                // Perna esquerda
                if (!this->legs) {
                    glPushMatrix();
                        glTranslatef(this->radius/3, -this->radius/1.4, 0);
                        drawRectangle(this->radius/3, this->radius/2, 0, 0, 0);                
                    glPopMatrix();
                }

                // Braco
                glPushMatrix();
                    glTranslatef(-(this->radius - 2), 0, 0);
                    glRotatef(-this->thetaArm, 0, 0, 1);

                    drawRectangleLine(this->radius/5, this->radius, 0, 0, 0);
                    drawRectangle(this->radius/5, this->radius, this->red, this->green, this->blue);

                    glTranslatef(0, this->radius, 0);
                glPopMatrix();

                drawEllipseLine(this->radius, this->radius/4, 0, 0, 0);
                drawEllipse(this->radius, this->radius/4, this->red, this->green, this->blue);

                drawCircleLine(this->radius/2, 0, 0, 0);
                drawCircle(this->radius/2, this->red, this->green, this->blue);
            glPopMatrix();
    glPopMatrix();
}