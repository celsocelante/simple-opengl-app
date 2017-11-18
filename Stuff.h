#include <math.h>
#include <list>
#include "Circle.h"
#include "Robot.h"
#include "Bullet.h"
#include "Enemy.h"

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif

#ifndef STUFF_H
#define STUFF_H

class Stuff {
    public:
        Stuff();
        GLint totalScore = 0;
        GLint totalEnemies = 0;
        bool gameEnded = false;

        Robot* bot;
        Circle* arena;
        Circle* center;

        list<Bullet*> enemyBullets;
        list<Bullet*> bullets;
        list<Circle*> obstacles;

        list<Enemy*> enemies;
};

#endif