#pragma once

#include <iostream>
#include "components/simple_scene.h"
#include "lab_m1/Duck-Hunt/duckUtils.h"

namespace duckShapes {

    inline void CreateMesh(const char* name,
        const std::vector<VertexFormat>& vertices,
        const std::vector<unsigned int>& indices,
        std::unordered_map<std::string, Mesh*>& meshes) {

        unsigned int VAO = 0;
        // Create the VAO and bind it
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        unsigned int VBO = 0;
        // Create the VBO and bind it
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // Send vertices data into the VBO buffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

        unsigned int IBO = 0;
        // Create the IBO and bind it
        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

        // Send indices data into the IBO buffer
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

        // Unbind the VAO
        glGenVertexArrays(0, &VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VAO);

        // Mesh information is saved into a Mesh object
        meshes[name] = new Mesh(name);
        meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    }

    inline void CreateCircle(float radius,
        glm::vec3 center,
        glm::vec3 color,
        std::string name,
        char *createMeshName,
        std::unordered_map<std::string, Mesh*>& meshes) {

        float twicePI = 2.0f * M_PI;

        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        // Filling the vertices
        vertices.push_back(
            VertexFormat(center, color));

        vertices.push_back(
            VertexFormat(glm::vec3(center[0], center[1] + radius, 0), color));

        for (int i = 1; i <= 360; i++) {
            vertices.push_back(VertexFormat(glm::vec3((center[0] + radius * sin(i * twicePI / 360)),
                (center[1] + radius * cos(i * twicePI / 360)), 0), color));
        }

        // Filling the indices
        for (int i = 1; i <= 360; i++) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
        }

        // Add the circle to the meshes
        meshes[name] = new Mesh(name);
        meshes[name]->InitFromData(vertices, indices);
        CreateMesh(createMeshName, vertices, indices, meshes);
    }

	inline void CreateTriangle(VertexFormat vertex0,
        VertexFormat vertex1,
        VertexFormat vertex2,
        std::string name,
        char* createMeshName,
        std::unordered_map<std::string, Mesh*>& meshes) {

        // Fill the vertices and indices
        std::vector<VertexFormat> vertices = { vertex0, vertex1, vertex2 };
        std::vector<unsigned int> indices = { 0, 1, 2 };

        // Add the triangle to the meshes
        meshes[name] = new Mesh(name);
        meshes[name]->InitFromData(vertices, indices);
        CreateMesh(createMeshName, vertices, indices, meshes);
	}

    inline void CreateRectangle(VertexFormat vertex0,
        VertexFormat vertex1,
        VertexFormat vertex2,
        VertexFormat vertex3,
        std::string name,
        char* createMeshName,
        std::unordered_map<std::string, Mesh*>& meshes) {

        // Fill the vertices and indices
        std::vector<VertexFormat> vertices = { vertex0, vertex1, vertex2, vertex3 };
        std::vector<unsigned int> indices = {0, 1, 2, 0, 2, 3};

        // Add the rectangle to the meshes
        meshes[name] = new Mesh(name);
        meshes[name]->InitFromData(vertices, indices);
        CreateMesh(createMeshName, vertices, indices, meshes);
    }

    inline void CreateBullet(VertexFormat vertex0,
        VertexFormat vertex1,
        VertexFormat vertex2,
        VertexFormat vertex3,
        VertexFormat vertex4,
        std::string name,
        char* createMeshName,
        std::unordered_map<std::string, Mesh*>& meshes) {

        // Fill the vertices and indices
        std::vector<VertexFormat> vertices = { vertex0, vertex1, vertex2, vertex3, vertex4 };
        std::vector<unsigned int> indices =
        {
            0, 1, 2,
            0, 2, 4,
            2, 3, 4
        };

        // Add the bullet to the meshes
        meshes[name] = new Mesh(name);
        meshes[name]->InitFromData(vertices, indices);
        CreateMesh(createMeshName, vertices, indices, meshes);
    }


    inline void CreateWireframe(VertexFormat vertex0, VertexFormat vertex1, VertexFormat vertex2,
        VertexFormat vertex3, VertexFormat vertex4, VertexFormat vertex5,
        VertexFormat vertex6, VertexFormat vertex7, VertexFormat vertex8,
        VertexFormat vertex9, VertexFormat vertex10, VertexFormat vertex11,
        std::string name,
        char* createMeshName,
        std::unordered_map<std::string, Mesh*>& meshes) {

        // Fill the vertices and indices
        std::vector<VertexFormat> vertices
        {
            vertex0, vertex1, vertex2, vertex3, vertex4, vertex5, vertex6, vertex7, 
            vertex8, vertex9, vertex10, vertex11
        };
        std::vector<unsigned int> indices =
        {
            0, 1, 2,
            0, 2, 3,

            4, 5, 6,
            4, 6, 7,

            4, 3, 8,
            4, 8, 9,

            10, 2, 5,
            10, 5, 11
        };

        // Add the wireframe to the meshes
        meshes[name] = new Mesh(name);
        meshes[name]->InitFromData(vertices, indices);
        CreateMesh(createMeshName, vertices, indices, meshes);
    }

    inline void CreateFire(std::unordered_map<std::string, Mesh*>& meshes,
        glm::ivec2 resolution) {

        // Create the fire made out of 3 circles and a triangle
        CreateTriangle(VertexFormat(glm::vec3(-7, 5, 0), glm::vec3(1, 0.4, 0)),
                       VertexFormat(glm::vec3(7, 5, 0), glm::vec3(1, 0.4, 0)),
                       VertexFormat(glm::vec3(0, 15, 0), glm::vec3(1, 0.4, 0)),
                       "fire-triangle", "created a fire triangle", meshes);
        CreateCircle(10, glm::vec3(0, 0, 0), glm::vec3(1, 0.4, 0), "fire-background", "created the fire background", meshes);
        CreateCircle(5, glm::vec3(-0.4, -1.3, 0), glm::vec3(1, 0.8, 0.4), "fire-inside", "created the fire inside", meshes);
        CreateCircle(2, glm::vec3(-0.4, -1.5, 0), glm::vec3(1, 1, 0.6), "fire-center", "created the fire center", meshes);
    }

    inline void CreateMeshes(std::unordered_map<std::string, Mesh*>& meshes,
        glm::ivec2 resolution) {

        // Draw the head of the duck
        CreateCircle(32, glm::vec3(40, 0, 0), glm::vec3(0.14, 0.3, 0), "head", "create the head", meshes);

        // Draw the body of the duck
        CreateTriangle(VertexFormat(glm::vec3(-70, 50, 0), glm::vec3(0.4, 0.2, 0)),
            VertexFormat(glm::vec3(30, 0, 0), glm::vec3(0.2, 0.1, 0)),
            VertexFormat(glm::vec3(-70, -50, 0), glm::vec3(0.4, 0.2, 0)),
            "duck_body", "created the body of the duck", meshes);
        
        // Draw the wings of the duck
        CreateTriangle(VertexFormat(glm::vec3(-30, 50, 0), glm::vec3(0.2, 0.1, 0)),
            VertexFormat(glm::vec3(-30, 5, 0), glm::vec3(0.4, 0.2, 0)),
            VertexFormat(glm::vec3(0, 5, 0), glm::vec3(0.4, 0.2, 0)),
            "duck_wing1", "created the wing1 of the duck", meshes);

        CreateTriangle(VertexFormat(glm::vec3(-30, -50, 0), glm::vec3(0.4, 0.2, 0)),
            VertexFormat(glm::vec3(-30, -5, 0), glm::vec3(0.2, 0.1, 0)),
            VertexFormat(glm::vec3(0, -5, 0), glm::vec3(0.2, 0.1, 0)),
            "duck_wing2", "created the wing2 of the duck", meshes);

        
        // Draw the beak of the duck
        CreateTriangle(VertexFormat(glm::vec3(60, 20, 0), glm::vec3(0.9, 0.9, 0)),
            VertexFormat(glm::vec3(100, 0, 0), glm::vec3(0.9, 0.9, 0)),
            VertexFormat(glm::vec3(60, -20, 0), glm::vec3(0.9, 0.9, 0)),
            "beak", "created the beak of the duck", meshes);

        // Draw the hearts - circles
        CreateCircle(7, glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), "heart", "created the hearts", meshes);

        // Draw the bullets
        CreateBullet(VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)),
            VertexFormat(glm::vec3(10, 0, 0), glm::vec3(0, 1, 0)),
            VertexFormat(glm::vec3(10, 40, 0), glm::vec3(0, 1, 0)),
            VertexFormat(glm::vec3(5, 50, 0), glm::vec3(0, 1, 0)),
            VertexFormat(glm::vec3(0, 40, 0), glm::vec3(0, 1, 0)),
            "bullet", "created the bullets", meshes);
       
        // Draw the ground
        CreateRectangle(VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 1, 0.8)),
            VertexFormat(glm::vec3(resolution.x, 0, 0), glm::vec3(1, 1, 0.8)),
            VertexFormat(glm::vec3(resolution.x, 150, 0), glm::vec3(0.9, 0.9, 0)),
            VertexFormat(glm::vec3(0, 150, 0), glm::vec3(0.9, 0.9, 0)),
            "ground", "created the ground", meshes);
        
        // Draw the score
        CreateRectangle(VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)),
            VertexFormat(glm::vec3(40, 0, 0), glm::vec3(0, 0, 1)),
            VertexFormat(glm::vec3(40, 30, 0), glm::vec3(0, 0, 1)),
            VertexFormat(glm::vec3(0, 30, 0), glm::vec3(0, 0, 1)),
            "score", "created the score bar", meshes);

        // Draw the score wireframe
        CreateWireframe(VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(203, 0, 0), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(203, 3, 0), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(0, 3, 0), glm::vec3(0, 0, 0)),

            VertexFormat(glm::vec3(0, 27, 0), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(203, 27, 0), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(203, 30, 0), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(0, 30, 0), glm::vec3(0, 0, 0)),

            VertexFormat(glm::vec3(3, 3, 0), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(3, 27, 0), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(200, 3, 0), glm::vec3(0, 0, 0)),
            VertexFormat(glm::vec3(200, 27, 0), glm::vec3(0, 0, 0)),
            "score-wireframe", "created the scorebar wireframe", meshes);

        // X for loosing
        CreateRectangle(VertexFormat(glm::vec3(-50, -20, 0), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(50, -20, 0), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(50, 20, 0), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(-50, 20, 0), glm::vec3(1, 0, 0)),
            "x-rectangle", "created the rectangle for loosing", meshes);

        // V for winning
        CreateRectangle(VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)),
            VertexFormat(glm::vec3(100, 0, 0), glm::vec3(0, 1, 0)),
            VertexFormat(glm::vec3(100, 20, 0), glm::vec3(0, 1, 0)),
            VertexFormat(glm::vec3(0, 20, 0), glm::vec3(0, 1, 0)),
            "v-rectangle", "created the rectangle for winning", meshes);

        // Draw the streak fire signs
        CreateFire(meshes, resolution);
    }
}  // namespace duckShapes