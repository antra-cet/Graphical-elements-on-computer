#pragma once

#include "lab_m1/Duck-Hunt/duckStructures.h"
#include "lab_m1/Duck-Hunt/duckUtils.h"
#include "lab_m1/Duck-Hunt/Duck-Hunt.h"
#include "components/simple_scene.h"

#include <ctime>

namespace duckMovement {

    inline void moveDuck(duckStructures::duckUtils &duck, float deltaTimeSeconds, glm::ivec2 resolution) {

        // If the duck has escaped or was hit, then make a new duck
        if ((duck.movementCase == duckStructures::DUCK_ESCAPE && duck.translateDuck.y + COLLISION_FACTOR_Y >= lastResolution.y)
            || (duck.movementCase == duckStructures::DUCK_HIT && duck.translateDuck.y - COLLISION_FACTOR_Y  <= 0)) {
            duckUtils::duckSetParameters(duck, resolution);
        }

        // If the time passed, make the duck escape and change the angle to go up
        if (duck.numBullets == 0 || duck.time >= 30) {
            duck.movementCase = duckStructures::DUCK_ESCAPE;
            duck.angle = 90;
        }

        // If the duck was hit, change the angle to go down
        if (duck.movementCase == duckStructures::DUCK_HIT) {
            duck.angle = -90;
        }

        // If the duck touched a x corner
        if (duck.translateDuck.x - COLLISION_FACTOR_X <= 0 || duck.translateDuck.x + COLLISION_FACTOR_X >= lastResolution.x) {
            duck.angle = 180 - duck.angle;
        }

        // If the duck touched a y corner
        if (duck.translateDuck.y - COLLISION_FACTOR_Y <= 0 || duck.translateDuck.y + COLLISION_FACTOR_Y >= lastResolution.y) {
            duck.angle = (-1) * duck.angle;
        }

        // Change the translation indices
        duck.translateDuck.x += deltaTimeSeconds * duck.speed * cos(duck.angle * M_PI / 180);
        duck.translateDuck.y += deltaTimeSeconds * duck.speed * sin(duck.angle * M_PI / 180);
        duck.time += deltaTimeSeconds;
    }
}  // namespace duckMovement