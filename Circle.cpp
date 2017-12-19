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
    glBegin(GL_QUAD_STRIP);
    glColor3f(1 - red, 1 - green, 1 - blue);
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
    glVertex3f(x + radius, y, -height);
    glVertex3f(x + radius, y, 0.0);
    glEnd();

    /** Draw the circle on top of cylinder */
    
    glBegin(GL_POLYGON);
    glColor3f(red, green, blue);
    angle = 0.0;
    for(GLint i = 0; i <= triangles; i ++) {
        glVertex3f(
            x + (radius * cos(i *  twicePi / triangles)),
            y + (radius * sin(i * twicePi / triangles)),
        height
        );
    }
    glVertex3f(radius, 0.0, -height);
    glEnd();
    
    /** Draw the circle on the bottom of cylinder */
    GLfloat materialEmission[] = { 0.10, 0.10, 0.10, 1};
    GLfloat materialColorD[] = { red, green, blue, 1};
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1};
    GLfloat mat_shininess[] = { 128.0 };
    GLfloat ambient[] = { 0.2, 0.2, 0.2, 1};
 
    glBegin(GL_POLYGON);
    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColorD);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
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
}

// Contorno
void Circle::drawMinimap() {
    GLfloat twicePi = 2.0f * M_PI;
    GLint triangles = 300;

    glLineWidth(0.5);
    glBegin(GL_LINE_LOOP);
    glColor3f(1, 1, 1);
		// glVertex3f(x, y, 0); // center of circle
		for(GLint i = 0; i <= triangles; i ++) { 
			glVertex3f(
		        x + (radius * cos(i *  twicePi / triangles)), 
			    y + (radius * sin(i * twicePi / triangles)), 0
			);
		}
	glEnd();
}

// Para obstaculos do minimapa
void Circle::drawObstacle2d() {
    GLfloat twicePi = 2.0f * M_PI;
    GLint triangles = 300;

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

void Circle::drawFloor(GLuint t) {
    GLfloat materialEmission[] = { 0, 0, 0, 1.0};
    GLfloat materialColor[] = { red, green, blue, 1.0};
    GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1};
    GLfloat mat_shininess[] = { 10.0 };
    GLfloat ambient[] = { 0.2, 0.2, 0.2, 1};

    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable(GL_TEXTURE_2D);
    GLUquadricObj *quadratic = gluNewQuadric();
    glPushMatrix();
        glBindTexture (GL_TEXTURE_2D, t);
        glTranslatef(x, y, 0);
        gluQuadricNormals(quadratic, GLU_SMOOTH);
        gluQuadricOrientation(quadratic, GLU_OUTSIDE);
        gluQuadricTexture(quadratic, GL_TRUE);
        gluDisk(quadratic, 0.0, radius, 100, 100);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Circle::drawObstacle(GLuint t) {

    glEnable(GL_TEXTURE_2D);
    GLfloat materialEmission[] = { 0, 0, 0, 1.0};
    GLfloat materialColor[] = { red, green, blue, 1.0};
    GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1};
    GLfloat mat_shininess[] = { 128.0 };
    GLfloat ambient[] = { 0.2, 0.2, 0.2, 1};

    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    GLUquadricObj *quadratic = gluNewQuadric();
    glPushMatrix();
        glBindTexture (GL_TEXTURE_2D, t);
        glTranslatef(x, y, 0);
        gluQuadricNormals(quadratic, GLU_SMOOTH);
        gluQuadricOrientation(quadratic, GLU_OUTSIDE);
        // glScalef(5.0f, 1.0f, 1.0f);
        gluQuadricTexture(quadratic, GL_TRUE);
        gluCylinder(quadratic, radius, radius, height, 100, 100);
        glTranslatef(0, 0, height);
        gluDisk(quadratic, 0.0, radius, 100, 100);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Circle::drawWall(GLuint t) {
    GLfloat materialEmission[] = { 0, 0, 0, 1.0};
    GLfloat materialColor[] = { red, green, blue, 1.0};
    GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1};
    GLfloat mat_shininess[] = { 128.0 };
    GLfloat ambient[] = { 0.2, 0.2, 0.2, 1};

    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable(GL_TEXTURE_2D);
    GLUquadricObj *quadratic = gluNewQuadric();
    glBindTexture (GL_TEXTURE_2D, t);
    glPushMatrix();
        glTranslatef(x, y, 0);
        gluQuadricNormals(quadratic, GLU_SMOOTH);
        gluQuadricOrientation(quadratic, GLU_OUTSIDE);
        // glScalef(5.0f, 1.0f, 1.0f);
        gluQuadricTexture(quadratic, GL_TRUE);
        gluCylinder(quadratic, radius, radius, height, 100, 100);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}


void Circle::drawWallArena(GLuint t) {
    // GLfloat materialEmission[] = { 1.0, 1.0, 1.0, 1};
    GLfloat materialColorA[] = { 0.2, 0.2, 0.2, 1};
    GLfloat materialColorD[] = { 1.0, 1.0, 1.0, 1};
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1};
    GLfloat mat_shininess[] = { 80.0 };
    glColor3f(1,1,1);

    // glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialColorA);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColorD);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable(GL_TEXTURE_2D);
    GLUquadricObj *quadratic = gluNewQuadric();
    glPushMatrix();
        glBindTexture (GL_TEXTURE_2D, t);
        glTranslatef(x, y, 0);
        gluQuadricNormals(quadratic, GLU_SMOOTH);
        gluQuadricOrientation(quadratic, GLU_OUTSIDE);
        // glScalef(5.0f, 1.0f, 1.0f);
        gluQuadricTexture(quadratic, GL_TRUE);
        gluCylinder(quadratic, radius, radius, height, 100, 100);
        glTranslatef(0, 0, height);
        gluDisk(quadratic, 0.0, radius, 100, 100);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
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

bool Circle::collision(Circle *c, GLfloat dx, GLfloat dy, GLfloat dz,
    GLfloat minHeight, GLfloat maxHeight) {

    GLfloat temp = sqrt(pow(c->getX() - (this->x + dx), 2) +
                            pow(c->getY() - (this->y + dy), 2));

	return temp <= (c->getRadius() + this->radius) && z >= minHeight && z <= maxHeight;
}

// bool Circle::collisionXY(Circle *c, GLfloat dx, GLfloat dy) {

//     GLfloat temp = sqrt(pow(c->getX() - (this->x + dx), 2) +
//                             pow(c->getY() - (this->y + dy), 2) +
//                             pow(c->getZ() - (this->z + dz), 2));

// 	return temp <= (c->getRadius() + this->radius) && z >= minHeight && z <= maxHeight;
// }

bool Circle::collisionNoDist(GLfloat x, GLfloat y, GLfloat z, GLfloat radius,
    GLfloat minHeight, GLfloat maxHeight) {
    
    GLfloat temp = sqrt(pow(x - this->x, 2) +
                            pow(y - this->y, 2));

	return temp <= (radius + this->radius) && z >= minHeight && z <= maxHeight;
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
