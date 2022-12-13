#pragma once

#include "utils/glm_utils.h"
#include "lab_m1/Car-Race/Car_Race.h"


#define CAR_SCALE 0.7
#define ROAD_DISTANCE 13

#define NUM_TREES 1000

#define LIGHT_POSITION_X 1000
#define LIGHT_POSITION_Y 1000
#define LIGHT_POSITION_Z 0

namespace m1
{
    class Car_Utils
    {
    
    public:
        struct car {
            glm::vec3 translate;
            float angle;

            unsigned int materialShininess;
            float materialKd;
            float materialKs;
        };

        struct road {
            std::vector<VertexFormat> vertices;
            std::vector<unsigned int> indices;
            glm::vec3 roadColor, normal;
            float distance;

            std::vector<glm::vec3> skelPoints;
        };

        struct landscape {
            glm::vec3 lightPosition;
        };
    };
}   // namespace m1
