#pragma once

#include "utils/glm_utils.h"

#define CAR_START_POINT_X 0
#define CAR_START_POINT_Y 0
#define CAR_START_POINT_Z 0

#define LIGHT_POSITION_X 1000
#define LIGHT_POSITION_Y 1000
#define LIGHT_POSITION_Z 0

namespace m1
{
    class Car_Utils
    {
    
    public:
        struct point {
            float x, y, z;
        };

        struct car {
            glm::vec3 translate;
            float angle;

            unsigned int materialShininess;
            float materialKd;
            float materialKs;
        };

        struct landscape {
            glm::vec3 lightPosition;
        };
    };
}   // namespace m1
