#include "Enemy.h"

void Enemy::update() {
    srand(time(NULL));
    GLint secret = rand() % 7 + 1;

    moveForward();

    // if (secret % 2 == 0) {
    //     rotateLeft();
    // } else if (secret % 7 == 0) {
    //     rotateRight();
    // }

}