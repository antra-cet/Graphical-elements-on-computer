#pragma once

namespace duckStructures {

    // Enum for duck cases of movement
    enum DuckMovementCases {
        DUCK_FLY, DUCK_ESCAPE, DUCK_HIT
    };

    struct point {
        float x, y;
    };

    struct bounds {
        point lower, upper;
    };

    struct duckWingMovement {
        int moveWings;

        point moveWing1, moveWing2;
        point translateWing1, translateWing2;
    };

    struct duckUtils {
        // Duck movement
        float angle;
        point translateDuck;
        DuckMovementCases movementCase;
        duckWingMovement wingMove;

        // Other game parameters
        float speed;
        float score;
        float time;
        int numBullets;
        int lives;

        int killingSpree;
    };

}  // namespace duckStructures