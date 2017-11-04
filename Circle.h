#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif

#include <math.h>

#ifndef CIRCLE_H
#define CIRCLE_H

class Circle {
    protected:
        GLfloat x, y, z;
        GLfloat radius;
        GLfloat previousRadius;

        GLfloat px, py, pz;
        
        bool displayed;
        bool beingDragged;
        bool beingResized;

        bool jumping;

        GLfloat red, green, blue;

        GLint id;
    
    public:
        Circle(GLfloat c_x, GLfloat c_y, GLfloat c_z, GLfloat c_r);
        Circle();

        void setX(GLfloat c_x);
        void setY(GLfloat c_y);
        void setZ(GLfloat c_z);
        void setCoord(GLfloat c_x, GLfloat c_y, GLfloat c_z);
        void setRadius(GLfloat c_r);
        void setDisplayed(bool d);
        void setDragState(bool bgd);
        void setResizeState(bool rs);
        void setRGB(GLfloat r, GLfloat g, GLfloat b);
        void draw();

        GLfloat getX() { return x; }
        GLfloat getY() { return y; }
        GLfloat getZ() { return z; }
        GLfloat getRadius() { return radius; }
        bool getDisplayed() { return displayed; }
        bool getDragState() { return beingDragged; }
        bool getResizeState() { return beingResized; }
        bool isJumping() { return jumping; }
        GLint getId() { return id; }

        void move(GLfloat x, GLfloat y, GLfloat z);
        bool collision(Circle *c, GLfloat dx, GLfloat dy);
        void setJumping(bool value);
        void setId(GLint id);
        void changeRadius(GLfloat i);
        void restoreRadius();

};
#endif