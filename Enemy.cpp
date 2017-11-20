#include "Enemy.h"

void Enemy::update() {
    srand(time(NULL));
    GLint secret = rand() % 7 + 1;

    moveForward();

    if (secret % 3 == 0) {
        rotateLeft();
    }

}