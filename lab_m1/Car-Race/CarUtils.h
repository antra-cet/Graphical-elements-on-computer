#pragma once

#include "utils/glm_utils.h"

namespace m1
{
    class Car_Utils
    {
    
    public:
        struct point {
            float x, y, z;
        };

        struct car {
            point translate;
            float rotateAngle;
        };
    };
}   // namespace m1
