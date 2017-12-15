#include "Robot.h"
#include "Stuff.h"

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

    // Object type
    this->type = 2;
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

void Robot::setHeight(GLfloat height) {
    this->height = height;
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


void Robot::rotateArmUp() {
    if (this->thetaArmZ < ARM_ANGLE) {
        this->thetaArmZ += ARM_MOVEMENT;
    }
}

void Robot::rotateArmDown() {
    if (this->thetaArmZ > -ARM_ANGLE) {
        this->thetaArmZ -= ARM_MOVEMENT;
    }
}


void Robot::rotateRight() {
    this->theta -= this->velocity * 3 * ROTATE;
}

void Robot::rotateLeft() {
    this->theta +=  this->velocity * 3 * ROTATE;
}

void Robot::swapLegs() {
    this->legControl += 1 * this->velocity;
    double sinLegControl = sin(this->legControl);
    double cosLegControl = cos(this->legControl);
    this->thetaLeftLeg1 = sinLegControl > 0 ? sinLegControl * 60 : 0;
    this->thetaLeftLeg2 = cosLegControl > 0 ? -this->thetaLeftLeg1 : -sinLegControl ;
    this->thetaRightLeg1 = -sinLegControl > 0 ? -sinLegControl * 60 : 0;
    this->thetaRightLeg2 = -cosLegControl > 0 ? -this->thetaRightLeg1 : sinLegControl ;
}

bool Robot::ableToMove(GLfloat dx, GLfloat dy, GLfloat dz) {
    srand (time(NULL));
    GLint secret = rand() % 10 + 1;

    if (this->type==3 && collision(stuff->bot, dx, dy)) {
        if (secret % 2 == 0) {
            rotateRight();
        } else {
            rotateLeft();
        }
        return false;
    }

    // Center collision
    if (collision(stuff->center, dx, dy)) {

        if(this->type == 3) {

            for (int i = 0; i < (int) (secret / 2); i++) {
                if (secret % 2 == 0) {
                    rotateRight();
                } else {
                    rotateLeft();
                }
            }
        }

        return false;
    }

    // Arena
    if( (sqrt(pow(stuff->arena->getX() - (x + dx), 2) + pow(stuff->arena->getY() - (y + dy), 2))
                                                             + radius) >= stuff->arena->getRadius()) {

        if(this->type == 3) {
            for (int i = 0; i < secret; i++) {
                if (secret % 2 != 0) {
                    rotateRight();
                } else {
                    rotateLeft();
                }
            }
        }

        return false;
    }

    // Black obstacles
    if (!isJumping() && disabledLowObstacle.getId() != -1 && sqrt(pow(disabledLowObstacle.getX() - (x + dx), 2) + 
                                    pow(disabledLowObstacle.getY() - (y + dy), 2)) > (disabledLowObstacle.getRadius() + radius) ) {
        setMoveFreely(false);
    }

    for (Circle* lo : stuff->obstacles) {
        if (collision(lo, dx, dy) && isJumping()) {
            setMoveFreely(true);

            // Last unlocked lowObstacle info
            disabledLowObstacle.setRadius(lo->getRadius());
            disabledLowObstacle.setX(lo->getX());
            disabledLowObstacle.setY(lo->getY());
            disabledLowObstacle.setId(lo->getId());
        }

        if (collision(lo, dx, dy) && !isJumping() && !canMoveFreely()) {

            if(this->type == 3 && secret % 2 == 0) {
                rotateLeft();
            } else {
                rotateRight();
            }

            // cout << "Jump, enemy!" << endl;
            return false;
        }
    }

    // Red obstacles
    for (Circle* e : stuff->enemies) {
        if (e->displayed && collision(e, dx, dy)) {

            if (this->type == 3) {
                if (this->id == e->getId()) {
                    continue;
                }
                rotateLeft();
            }

            return false;
        }
    }

    return true;
}

GLfloat Robot::newX() {
    return cos((this->theta - 90) * M_PI / 180) * this->velocity * MOVEMENT;
}

GLfloat Robot::newY() {
    return sin((this->theta - 90) * M_PI / 180) * this->velocity * MOVEMENT;
}

void Robot::moveForward() {

    GLfloat newx = x - newX();
    GLfloat newy = y - newY();

    if (ableToMove(newx - x, newy - y, 0)) {
        x -= newX();
        y -= newY();
        swapLegs();

    }
}

void Robot::moveBackward() {
    GLfloat newx = x + newX();
    GLfloat newy = y + newY();

    if (ableToMove(newx - x, newy - y, 0)) {
        x += newX();
        y += newY();
        swapLegs();
    }
}

void Robot::setScale(GLfloat scale) {
    this->scale = scale;
}

void Robot::changeScale(GLfloat i) {
    this->scale += i;
}

void Robot::setFire() {

    // if (!canMoveFreely() && !isJumping() && this->type == 2) {
    //     Bullet* b = new Bullet(x, y, thetaArm, theta, bulletVelocity, radius, height);
    //     stuff->bullets.push_back(b);

    //     return;
    // }

    // if (this->type == 3) {
    //     Bullet* b = new Bullet(x, y, thetaArm, theta, bulletVelocity, radius, height);
    //     stuff->enemyBullets.push_back(b);
    // }

}

void Robot::jumpStart(GLint v) {
    setJumping(true);
    GLfloat r = radius;

    changeRadius(r * (FACTOR/ANIMATION_FRAMES));
    changeScale(FACTOR/ANIMATION_FRAMES);
}

void Robot::jumpMiddle(GLint v) {
    GLfloat r = radius;

    changeRadius( -(r * (FACTOR/ANIMATION_FRAMES)) ); 
    changeScale(-((FACTOR/ANIMATION_FRAMES)));
}

void Robot::jumpEnd(GLint v) {
    setJumping(false);
    restoreRadius();
    restoreScale();
}

void Robot::jump() {
    if (!isJumping()) {
        // for (GLint i = 1; i <= ANIMATION_FRAMES; i++) {
        //     glutTimerFunc( ((ANIMATION_TIME/2) / ANIMATION_FRAMES) * i, jumpStart, 0);
        // }

        // for (GLint i = 1; i <= ANIMATION_FRAMES; i++) {
        //     glutTimerFunc(ANIMATION_TIME/2 + ((ANIMATION_TIME/2) / ANIMATION_FRAMES) * i, jumpMiddle, 0);
        // }

        // // Hold on for 2 seconds
        // glutTimerFunc(ANIMATION_TIME, jumpEnd, 0);
    }
}

void Robot::getGunPositionX() {

}

void Robot::getGunPositionY() {
    
}

void Robot::getGunPositionZ() {

}


void Robot::draw(int i) {

    GLfloat mat_ambient[] = { this->red, this->green, this->blue, 1.0 };
    GLfloat mat_ambient_g[] = { 0, 1, 0, 1.0 };
    GLfloat mat_ambient_b[] = { 0, 0, 1, 1.0 };


    glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(this->theta, 0, 0 ,1);

        glPushAttrib(GL_ENABLE_BIT);

            glDisable(GL_LIGHTING);
            glDisable(GL_TEXTURE_2D);
            glColor3fv(mat_ambient);
                           
            //Perna direita 
            glPushMatrix();
    
                glTranslatef(this->radius/2, 0, this->radius);
                glRotatef(this->thetaRightLeg1, 1, 0, 0);
                glTranslatef(0, 0, -this->radius/4);
                
                glPushMatrix();
                    glScalef(this->radius/7, this->radius/7, this->radius/2);
                    glutSolidCube(1.0);
                glPopMatrix();


                glTranslatef(0, 0, -this->radius/4);
                glRotatef(this->thetaRightLeg2, 1, 0, 0);
                glTranslatef(0, 0, -this->radius/4);

                glScalef(this->radius/7, this->radius/7, this->radius/2);
                glutSolidCube(1.0);

            glPopMatrix();

            //Perna esquerda
            glPushMatrix();

                glTranslatef(-this->radius/2, 0, this->radius);
                glRotatef(this->thetaLeftLeg1, 1, 0, 0);
                glTranslatef(0, 0, -this->radius/4);

                glPushMatrix();
                    glScalef(this->radius/7, this->radius/7, this->radius/2);
                    glutSolidCube(1.0);
                glPopMatrix();

                glTranslatef(0, 0, -this->radius/4);
                glRotatef(this->thetaLeftLeg2, 1, 0, 0);
                glTranslatef(0, 0, -this->radius/4);

                glScalef(this->radius/7, this->radius/7, this->radius/2);
                glutSolidCube(1.0);

            glPopMatrix();

            glTranslatef(0, 0, this->radius * 1.5);
        
            //Corpo
            glPushMatrix();

                glScalef(this->radius, this->radius/7, this->radius);
                glutSolidCube(1.0);

            glPopMatrix();

            //Braco direito
            glPushMatrix();

                glTranslatef(this->radius/2, this->radius/2, this->radius/2);
                glRotatef(this->thetaArm, 0, 0, 1);
                glRotatef(this->thetaArmZ, 1, 0, 0);
                glTranslatef(0, this->radius/2, 0);
                glScalef(this->radius/7, this->radius, this->radius/7);
                glutSolidCube(1.0);

            glPopMatrix();

            //Braco esquerdo
            glPushMatrix();

                glTranslatef(-this->radius/2, this->radius/2, this->radius/2);
                glScalef(this->radius/7, this->radius, this->radius/7);
                glRotatef(-90, 0, 1, 0);
                glutSolidCube(1.0);

            glPopMatrix();


            if(i != 1){
                //Cabeca
                glPushMatrix();

                    glTranslatef(0, 0, this->radius);
                    glScalef(this->radius/2, this->radius/2, this->radius/2);
                    glutSolidSphere(1.0, 360, 360);

                glPopMatrix();
            }


        glPopAttrib();

    glPopMatrix();
}