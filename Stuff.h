#include <math.h>
#include <list>
#include "Robot.h"
#include "Bullet.h"
#include "Enemy.h"

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#ifndef STUFF_H
#define STUFF_H

class Circle;

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