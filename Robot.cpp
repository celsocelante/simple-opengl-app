#include "Robot.h"

Robot::Robot() {}

Robot::Robot(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->radius = 20;
    this->i = 1;
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
    int triangles = 300;

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

void Robot::drawEllipse(double xRadius, double YRadius) {
    int i;

    glBegin(GL_TRIANGLE_FAN);
    // glBegin(GL_LINE_LOOP);
    glColor3f(0, 1, 0);
    for (i = 0; i < 360; i++) {
        double rad = i * (M_PI/180);
        glVertex2f(cos(rad) * xRadius, sin(rad) * YRadius);
    }
    glEnd();
}

// void Robot::rotateArmRight() {
//     if (this->thetaArm < 45) {
//         this->thetaArm += 1;
//     }
// }

// void Robot::rotateArmLeft() {
//     if (this->thetaArm > 0) {
//         this->thetaArm -= 1;
//     }
// }

void Robot::rotateRight() {
    this->theta += 1;

    if (theta >= 360) {
        theta = 0;
    }

    cout << theta << endl;
}

void Robot::rotateLeft() {
    this->theta -= 1;

    if (theta <= 0) {
        theta = 360;
    }

    cout << theta << endl;
}

void Robot::moveForward() {
    y -= abs(sin(theta));
    x -= abs(cos(theta));
}

void Robot::moveBackward() {
    this->y += 2;
    this->i += 1;
}

void Robot::draw() {
    glPushMatrix();
    glTranslatef(0 + i, 0 + i, 0);
        glPushMatrix();
            glTranslatef(x, y, 0);
            glRotatef(this->theta, 0, 0, 1);

            drawEllipse(this->radius, this->radius/4);

            // Perna direita
            glPushMatrix();
            glTranslatef(-this->radius/3, this->radius/5, 0);
            drawRectangle(this->radius/3, this->radius/2, 0, 0, 0);
            glPopMatrix();

            // Perna esquerda
            glPushMatrix();
                glTranslatef(this->radius/3, -this->radius/1.4, 0);
                drawRectangle(this->radius/3, this->radius/2, 0, 0, 0);
            glPopMatrix();

            // Braco
            glPushMatrix();
                glRotatef(this->thetaArm, 0, 0, 1);
                glTranslatef(-(this->radius - 0.4), 0, 0);
                drawRectangle(this->radius/3, this->radius, 0, 1, 0);
                glTranslatef(0, this->radius, 0);
            glPopMatrix();
        glPopMatrix();

        drawCircle(this->radius/2, 0, 1, 0);
    glPopMatrix();
}