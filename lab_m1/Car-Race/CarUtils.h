#pragma once

#include "utils/glm_utils.h"
#include "lab_m1/Car-Race/Car_Race.h"

#include <iostream>
#include<cmath>


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

        struct landscape {
            glm::vec3 lightPosition;
        };
    };

    inline bool isInTriangle(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, glm::vec3 point) {
        glm::vec3 v0, v1, v2;
        float d00, d01, d02, d11, d12;
        v0 = point3 - point1;
        v1 = point2 - point1;
        v2 = point - point1;

        // Compute dot products
        d00 = glm::dot(v0, v0);
        d01 = glm::dot(v0, v1);
        d02 = glm::dot(v0, v2);
        d11 = glm::dot(v1, v1);
        d12 = glm::dot(v1, v2);

        // Compute barycentric coordinates
        float invDenom = 1 / (d00 * d11 - d01 * d01);
        float u = (d11 * d02 - d01 * d12) * invDenom;
        float v = (d00 * d12 - d01 * d02) * invDenom;

        return (u >= 0) && (v >= 0) && (u + v < 1);
    }

    inline bool isOnTrack(glm::vec3 point, std::vector<glm::vec3> roadTriangles) {
        glm::vec3 point1, point2, point3;
        
        for (int i = 0; i < roadTriangles.size(); i++) {
            if (i == roadTriangles.size() - 1) {
                point1 = roadTriangles[i];
                point2 = roadTriangles[0];
                point3 = roadTriangles[1];
            }
            else {
                if (i == roadTriangles.size() - 2) {
                    point1 = roadTriangles[i];
                    point2 = roadTriangles[i + 1];
                    point3 = roadTriangles[0];
                }
                else {
                    point1 = roadTriangles[i];
                    point2 = roadTriangles[i + 1];
                    point3 = roadTriangles[i + 2];
                }
            }

            if (isInTriangle(point1, point2, point3, point)) {
                return 1;
            }
        }

        return 0;
    }
}   // namespace m1
