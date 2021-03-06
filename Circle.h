#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include <math.h>

#ifndef CIRCLE_H
#define CIRCLE_H

class Stuff;

class Circle {
    protected:
        GLfloat x, y, z;
        GLfloat radius, height;
        GLfloat previousRadius;

        GLfloat px, py, pz;
    
        bool beingDragged;
        bool beingResized;

        bool jumping;

        GLfloat red, green, blue;

        GLint id;
        
        Stuff* stuff;
    public:
        Circle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLfloat height);
        Circle();

        bool displayed = true;

        void setX(GLfloat x);
        void setY(GLfloat y);
        void setZ(GLfloat z);
        void setCoord(GLfloat x, GLfloat y, GLfloat z);
        void setRadius(GLfloat radius);
        void setDisplayed(bool d);
        void setDragState(bool bgd);
        void setResizeState(bool rs);
        void setRGB(GLfloat r, GLfloat g, GLfloat b);
        void draw();
        void drawFloor(GLuint texture);
        void drawObstacle(GLuint texture);
        void drawWall(GLuint texture);
        void drawWallArena(GLuint texture);
        void drawMinimap();
        void drawObstacle2d();

        GLfloat getX() { return x; }
        GLfloat getY() { return y; }
        GLfloat getZ() { return z; }
        GLfloat getRadius() { return radius; }
        bool getDisplayed() { return displayed; }
        bool getDragState() { return beingDragged; }
        bool getResizeState() { return beingResized; }
        bool isJumping() { return jumping; }
        GLint getId() { return id; }
        GLfloat getHeight() { return height; }

        void move(GLfloat x, GLfloat y, GLfloat z);
        bool collision(Circle *c, GLfloat dx, GLfloat dy, GLfloat dz, GLfloat minHeight, GLfloat maxHeight);
        bool collisionNoDist(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLfloat minHeight, GLfloat maxHeight);
        void setJumping(bool value);
        void setId(GLint id);
        void changeRadius(GLfloat i);
        void restoreRadius();
        void setStuff(Stuff* stuff);
        
        int type = 1;

};
#endif