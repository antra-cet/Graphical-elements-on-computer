#pragma once

#include "utils/glm_utils.h"
#include "lab_m1/Car-Race/Car_Race.h"

#include <iostream>
#include<cmath>

// CAR DEFINES
#define CAR_SCALE 1
#define CAR_FRONT 0.5
#define CAR_SIDE 1
#define CAR_MAX_SPEED 40
#define CAR_STOP 0
#define CAR_SPEED_INC 0.1

#define CAR_CAMERA_START_X 0
#define CAR_CAMERA_START_Y 3
#define CAR_CAMERA_START_Z 6.5f

// ENEMY CAR DEFINES
#define ENEMY_CARS_NUM 4
#define ENEMY_CARS_DIST 4
#define ENEMY_CARS_MOVEMENT 100
#define ENEMY_CAR_SPACING 0.2

// LANDSCAPE DEFINES
#define ROAD_WIDTH 13
#define NUM_TREES 1000

// MINIMAP DEFINES
#define MINIMAP_SIZE_X 10
#define MINIMAP_SIZE_Y 10
#define MINIMAP_PROP 5

namespace m1
{
    class Car_Utils
    {
    
    public:
        struct car {
            glm::vec3 translate;
            float angle;

            float enemyXTranslate, indexPoint, moveX, moveZ;

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

    inline glm::vec3 getActualTargetPosition(glm::vec3 targetPosition) {
        return glm::vec3(targetPosition.x - CAR_FRONT, targetPosition.y, targetPosition.z - CAR_FRONT);
    }

    inline std::vector<Car_Utils::car> getEnemyCars(std::vector<glm::vec3> moveEnemyCarPoints) {
        std::vector<Car_Utils::car> enemyCars;

        for (int i = 0; i < ENEMY_CARS_NUM; i++) {
            Car_Utils::car newEnemyCar{};
            newEnemyCar.indexPoint = rand() % moveEnemyCarPoints.size();
            newEnemyCar.translate = moveEnemyCarPoints[newEnemyCar.indexPoint];

            int sgn = (rand() % 2 == 0) ? 1 : -1;
            newEnemyCar.enemyXTranslate = sgn * (rand() % ENEMY_CARS_DIST);
            newEnemyCar.translate.x += newEnemyCar.enemyXTranslate;
            newEnemyCar.angle = 0;

            enemyCars.push_back(newEnemyCar);
        }

        return enemyCars;
    }
}   // namespace m1
