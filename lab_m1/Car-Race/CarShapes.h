#include "lab_m1/Car-Race/Car_Race.h"
#include "lab_m1/Car-Race/CarUtils.h"

namespace Car_Shapes {

    inline Mesh* CreateGrass() {
        // Create grass from multiple triangles so that
        // shaders apply smoothly
        glm::vec3 grassColor = glm::vec3(0, 0.7, 0.3);
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        int inc = 10;
        int start = -400;
        int end = 400;
        float splitFactor = (end - start) / inc + 1;

        for (int i = start; i <= end; i += inc) {
            for (int j = start; j <= end; j += inc) {
                // Choose random color between red, blue, green and yellow
                glm::vec3 color;
                int randomColor = rand() % 4;
                switch (randomColor) {
                    case 0:
                        color = glm::vec3(1, 0, 0);
                        break;
                    case 1:
                        color = glm::vec3(0, 1, 0);
                        break;
                    case 2:
                        color = glm::vec3(0, 0, 1);
                        break;
                    case 3:
                        color = glm::vec3(1, 1, 0);
                        break;
                    default:
                        color = glm::vec3(1, 1, 1);
                        break;
                }

                // Add the vertex to the vector
                vertices.push_back(VertexFormat(glm::vec3(i, 0, j), color));

                int iIndice = (i - start) / inc;
                int jIndice = (j - start) / inc;

                // If adding the first row, then there is no triangle to
                // add to the indexes vector
                if (iIndice == 0) {
                    continue;
                }

                // If adding the second row of indexes, then start making the
                // triangles

                // If on the first element of the row, no square can be fromed
                if (jIndice == 0) {
                    continue;
                }

                // Calculate upper and upper right indexes
                int index = iIndice * splitFactor + jIndice;
                int leftIndex = iIndice * splitFactor + (jIndice - 1);
                int upperIndex = (iIndice - 1) * splitFactor + jIndice;
                int upperLeftIndex = (iIndice - 1) * splitFactor + (jIndice - 1);

                // Add the two triangles
                indices.push_back(leftIndex);
                indices.push_back(upperLeftIndex);
                indices.push_back(upperIndex);

                indices.push_back(leftIndex);
                indices.push_back(upperIndex);
                indices.push_back(index);
            }
        }

        Mesh* mesh = new Mesh("grass");
        mesh->InitFromData(vertices, indices);

        return mesh;
    }


    inline std::vector<glm::vec3> CreateTrackPoints() {
        std::vector<glm::vec3> skelPoints = {
        glm::vec3(0, 0, 0), // A
        glm::vec3(0, 0, 22.0368), // F2
        glm::vec3(0, 0, 44.06), // B
        glm::vec3(0, 0, 68.3664), // E2
        glm::vec3(0, 0, 100), // D2
        glm::vec3(0, 0, 114.425), // C
        glm::vec3(5.92, 0, 142.498), // D
        glm::vec3(28.299, 0, 153.685), // E
        glm::vec3(57.790, 0, 151.651), // F
        glm::vec3(73.044, 0, 142.498), // G
        glm::vec3(75.078, 0, 124.1939), // H
        glm::vec3(77.1125, 0, 105.889), // I
        glm::vec3(77.112, 0, 63.1776),// J
        glm::vec3(79.824, 0, 30.4104),// G2
        glm::vec3(77.112, 0, 0), // K 
        glm::vec3(75.7206, 0, -28.0624), // H2
        glm::vec3(73.7175, 0, -58.2683), // L
        glm::vec3(81.4527, 0, -82.0335), // M
        glm::vec3(100, 0, -100), // N
        glm::vec3(124.199, 0, -102.3381), // O
        glm::vec3(146.6413, 0, -76.69), // P
        glm::vec3(168.0462, 0, -44.4759), // I2
        glm::vec3(179.7699, 0, 0), // Q
        glm::vec3(174.2013, 0, 38.61712), // W1
        glm::vec3(170.09797, 0, 63.23728), // K2
        glm::vec3(172.758, 0, 95.6068), // V1
        glm::vec3(168.046, 0, 126.839), // J2
        glm::vec3(174.4266, 0, 156.278), // R
        glm::vec3(174.426, 0, 177.6522), // S
        glm::vec3(160.534, 0, 187.27), // T
        glm::vec3(142.366, 0, 190.476), // U
        glm::vec3(100.22, 0, 193.41424), // T1
        glm::vec3(50.461, 0, 193.682), // V
        glm::vec3(0, 0, 193.414), // S1
        glm::vec3(-55.336, 0, 194.7508), // W
        glm::vec3(-121.568, 0, 197.32), // U1
        glm::vec3(-179.301, 0, 194.7508), // Z
        glm::vec3(-232.031, 0, 193.518), // Z1
        glm::vec3(-276.55, 0, 193.682), // A1
        glm::vec3(-298.992, 0, 180.858), // B1
        glm::vec3(-315.0222, 0, 161.6222), // C1
        glm::vec3(-319.296, 0, 139.18), // D1
        glm::vec3(-307.541, 0, 124.2189), // E1
        glm::vec3(-282.96, 0, 107.12), // F1
        glm::vec3(-270.138, 0, 95.3649), // G1
        glm::vec3(-273.34, 0, 76.128), // H1
        glm::vec3(-296.8549, 0, 59.0303), // I1
        glm::vec3(-321.4342, 0, 45.1376), // J1
        glm::vec3(-321.43, 0, 24.833), // K1
        glm::vec3(-300, 0, 0), // L1
        glm::vec3(-279.219, 0, -32.165), // A2
        glm::vec3(-257.314, 0, -55.3169),  // M1
        glm::vec3(-214.567, 0, -91.651), // N1
        glm::vec3(-200, 0, -100), // O1
        glm::vec3(-173.958, 0, -101.269),  // P1
        glm::vec3(-144.0357, 0, -87.376), // Q1
        glm::vec3(-104.827, 0, -63.966), // B2
        glm::vec3(-75.641, 0, -32.8749), // R1
        glm::vec3(-30.966, 0, -10.6231) // C2
        };

        return skelPoints;
    }

    inline std::vector<glm::vec3> CreateEnemyTrackPoints(std::vector<glm::vec3> skelPoints) {
        std::vector<glm::vec3> enemyCarPoints;

        for (int i = 0; i < skelPoints.size(); i++) {
            glm::vec3 currPoint, nextPoint;
            currPoint = skelPoints[i];
            nextPoint = skelPoints[(i + 1) % skelPoints.size()];

            enemyCarPoints.push_back(currPoint);
            for (int j = 0; j < ENEMY_CARS_MOVEMENT; j++) {
                glm::vec3 point;

                point.x = currPoint.x + (nextPoint.x - currPoint.x) * j / ENEMY_CARS_MOVEMENT;
                point.y = 0;
                point.z = currPoint.z + (nextPoint.z - currPoint.z) * j / ENEMY_CARS_MOVEMENT;

                enemyCarPoints.push_back(point);
            }
        }

        return enemyCarPoints;
    }

    inline std::vector<glm::vec3> CreateTrainglePoints(std::vector<glm::vec3> skelPoints, float distance) {
        std::vector<glm::vec3> trianglePoints;

        glm::vec3 v1, v2;
        glm::vec3 d, p;
        glm::vec3 red, blue;

        for (int i = 0; i < skelPoints.size(); i++) {
            if (i == skelPoints.size() - 1) {
                // Adding the last vertice
                v1 = skelPoints[skelPoints.size() - 1];
                v2 = skelPoints[0];

                d = v2 - v1;
                d = glm::normalize(d);
                p = glm::cross(d, glm::vec3(0, 1, 0));

                red = v1 + distance * p;
                blue = v1 - distance * p;
                trianglePoints.push_back(red);
                trianglePoints.push_back(blue);

                break;
            }


            // Adding the rest of the vertices
            v1 = skelPoints[i];
            v2 = skelPoints[i + 1];

            d = v2 - v1;
            d = glm::normalize(d);
            p = glm::cross(d, glm::vec3(0, 1, 0));

            red = v1 + distance * p;
            blue = v1 - distance * p;
            trianglePoints.push_back(red);
            trianglePoints.push_back(blue);
        }

        return trianglePoints;
    }

    inline std::vector<VertexFormat> CreateRoadVertices (std::vector<glm::vec3> skelPoints, float distance, glm::vec3 roadColor) {
        std::vector<VertexFormat> vertices;

        glm::vec3 v1, v2;
        glm::vec3 d, p;
        glm::vec3 red, blue;

        for (int i = 0; i < skelPoints.size(); i++) {
            if (i == skelPoints.size() - 1) {
                // Adding the last vertice
                v1 = skelPoints[skelPoints.size() - 1];
                v2 = skelPoints[0];

                d = v2 - v1;
                d = glm::normalize(d);
                p = glm::cross(d, glm::vec3(0, 1, 0));

                red = v1 + distance * p;
                blue = v1 - distance * p;
                vertices.push_back(VertexFormat(red, roadColor));
                vertices.push_back(VertexFormat(blue, roadColor));

                break;
            }


            // Adding the rest of the vertices
            v1 = skelPoints[i];
            v2 = skelPoints[i + 1];

            d = v2 - v1;
            d = glm::normalize(d);
            p = glm::cross(d, glm::vec3(0, 1, 0));

            red = v1 + distance * p;
            blue = v1 - distance * p;
            vertices.push_back(VertexFormat(red, roadColor));
            vertices.push_back(VertexFormat(blue, roadColor));
        }

        return vertices;
    }

    inline std::vector<unsigned int> CreateRoadIndices (std::vector<glm::vec3> skelPoints, float distance) {

        std::vector<unsigned int> indices;

        for (int i = 0; i < skelPoints.size(); i++) {
            if (i == skelPoints.size() - 1) {
                // Adding the triangles indices
                // our current indice
                int index = (skelPoints.size() - 1) * 2;

                // The first triangle
                indices.push_back(index - 2);
                indices.push_back(index - 1);
                indices.push_back(index);

                // The second triangle
                indices.push_back(index - 1);
                indices.push_back(index + 1);
                indices.push_back(index);

                // The third triangle
                indices.push_back(index);
                indices.push_back(index + 1);
                indices.push_back(0);

                // The fourth triangle
                indices.push_back(index + 1);
                indices.push_back(1);
                indices.push_back(0);

                break;
            }

            // Adding the triangles indices
            // our current indice
            int index = i * 2;

            // The first triangle
            indices.push_back(index - 2);
            indices.push_back(index - 1);
            indices.push_back(index);

            // The second triangle
            indices.push_back(index - 1);
            indices.push_back(index + 1);
            indices.push_back(index);
        }

        return indices;
    }

    inline Mesh *CreateRoad() {
        glm::vec3 normal = glm::vec3(1, 1, 1);
        glm::vec3 roadColor = glm::vec3(0.8, 0.8, 0.8);

        std::vector<glm::vec3> skelPoints = CreateTrackPoints();
        std::vector<VertexFormat> vertices = CreateRoadVertices(skelPoints, ROAD_WIDTH, roadColor);
        std::vector<unsigned int> indices = CreateRoadIndices(skelPoints, ROAD_WIDTH);

        Mesh* mesh = new Mesh("road");
        mesh->InitFromData(vertices, indices);

        return mesh;
	}

    inline std::vector<glm::vec3> CreateTreePoints(std::vector<glm::vec3> roadTriangles) {
        std::vector<glm::vec3> treePoints;
        for (int i = 0; i < NUM_TREES; i++) {
            int x = rand() % 400;
            int sgnX = rand() % 2;
            sgnX = (sgnX == 0) ? 1 : -1;
            x *= sgnX;

            int z = rand() % 400;
            int sgnZ = rand() % 2;
            sgnZ = (sgnZ == 0) ? 1 : -1;
            z *= sgnZ;

            if (!m1::isOnTrack(glm::vec3(x, 0, z), roadTriangles)) {
                treePoints.push_back(glm::vec3(x, 0, z));
            }
        }

        return treePoints;
    }
}