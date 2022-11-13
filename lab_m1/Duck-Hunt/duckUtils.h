#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Duck-Hunt/duckStructures.h"
#include "lab_m1/Duck-Hunt/duckShapes.h"
#include "lab_m1/Duck-Hunt/Duck-Hunt.h"

#define COLLISION_FACTOR_X 40
#define COLLISION_FACTOR_Y 40
#define LOWER_BOUND_Y 150
#define LOWER_BOUND_WING_X -1
#define UPPER_BOUND_WING_X 5

#define WING_SPEED 50

#define DUCK_INITIAL_SPEED 200
#define DUCK_INITIAL_LIVES 3
#define DUCK_INITIAL_SCORE 0
#define DUCK_INITIAL_NUMBER_OF_BULLETS 3

#define MAX_SCORE 4.7

float twicePI = 2.0f * M_PI;
glm::ivec2 lastResolution;

namespace duckUtils {

    inline void duckSetParameters(duckStructures::duckUtils &duck, glm::ivec2 resolution) {
        // If the duck escaped, then change parameters
        if (duck.movementCase == duckStructures::DUCK_ESCAPE) {
            duck.lives--;
            duck.speed *= 1.25;

            duck.killingSpree = 0;
        }

        // If the duck was hit, as well
        if (duck.movementCase == duckStructures::DUCK_HIT) {
            if (duck.score <= MAX_SCORE) {
                duck.score += 0.001 * duck.speed + 0.01 * duck.killingSpree;
            }

            duck.speed *= 1.25;
        }


        // Initialized the angle for the duck
        duck.angle = rand() % 45 + 30;
        int optuse = rand() % 2;
        if (optuse) {
            duck.angle += 90;
        }

        // Initialized the other game parameters
        duck.movementCase = duckStructures::DUCK_FLY;
        duck.translateDuck.x = resolution.x / 2;
        duck.translateDuck.y = 50;
        duck.time = 0;
        duck.numBullets = DUCK_INITIAL_NUMBER_OF_BULLETS;

        // Initialized parameters for the movement of the wings
        duck.wingMove.moveWings = 1;

        duck.wingMove.translateWing1.x = 0;
        duck.wingMove.translateWing1.y = 0;
        duck.wingMove.translateWing2.x = 0;
        duck.wingMove.translateWing2.y = 0;

        duck.wingMove.moveWing1.x = 0;
        duck.wingMove.moveWing2.x = 0;
    }
}   // namespace duckUtils
