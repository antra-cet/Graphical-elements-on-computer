
namespace carShapes {

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

    inline Mesh *CreateRoad() {

        glm::vec3 roadColor = glm::vec3(1, 1, 1);
        glm::vec3 normal = glm::vec3(1, 1, 1);
        float distance = 0.3;

        std::vector<glm::vec3> skelPoints = {
            glm::vec3(0, 0, 0),
            glm::vec3(8, 0, 29),
            glm::vec3(30, 0, 50),
            glm::vec3(59, 0, 39),
            glm::vec3(84, 0, 17),
            glm::vec3(119, 0, -34),
            glm::vec3(160, 0, -27),
            glm::vec3(189, 0, 14),
            glm::vec3(227, 0, 107),
            glm::vec3(195, 0, 208),
            glm::vec3(110, 0, 225),
            glm::vec3(79, 0, 193),
            glm::vec3(11, 0, 140),
            glm::vec3(-20, 0, 139),
            glm::vec3(-61, 0, 192),
            glm::vec3(-123, 0, 263),
            glm::vec3(-194, 0, 274),
            glm::vec3(-300, 0, 250),
            glm::vec3(-312, 0, 104),
            glm::vec3(-255, 0, -21),
            glm::vec3(-232, 0, -43),
            glm::vec3(-201, 0, -39),
            glm::vec3(-204, 0, -19),
            glm::vec3(-219, 0, 8),
            glm::vec3(-264, 0, 126),
            glm::vec3(-240, 0, 168),
            glm::vec3(-226, 0, 141),
            glm::vec3(-223, 0, 104),
            glm::vec3(-163, 0, -22),
            glm::vec3(-158, 0, -82),
            glm::vec3(-191, 0, -127),
            glm::vec3(-239, 0, -187),
            glm::vec3(-262, 0, -251),
            glm::vec3(-194, 0, -288),
            glm::vec3(-73, 0, -281),
            glm::vec3(17, 0, -221),
            glm::vec3(97, 0, -167),
            glm::vec3(129, 0, -82),
            glm::vec3(33, 0, -18),
        };

        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        // Adding the first 2 verices
        glm::vec3 v1 = skelPoints[0];
        glm::vec3 v2 = skelPoints[1];

        glm::vec3 d = v2 - v1;
        glm::vec3 p = glm::cross(d, glm::vec3(0, 1, 0));

        glm::vec3 red = v1 + distance * p;
        glm::vec3 blue = v1 - distance * p;
        vertices.push_back(red);
        vertices.push_back(blue);

        for (int i = 1; i < skelPoints.size() - 1; i++) {
            // Adding the rest of the vertices
            v1 = skelPoints[i];
            v2 = skelPoints[i + 1];

            d = v2 - v1;
            p = glm::cross(d, glm::vec3(0, 1, 0));

            red = v1 + distance * p;
            blue = v1 - distance * p;
            vertices.push_back(red);
            vertices.push_back(blue);

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

        // Adding the last vertice
        v1 = skelPoints[skelPoints.size() - 1];
        v2 = skelPoints[0];

        d = v2 - v1;
        p = glm::cross(d, glm::vec3(0, 1, 0));

        red = v1 + distance * p;
        blue = v1 - distance * p;
        vertices.push_back(red);
        vertices.push_back(blue);

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

        // The first triangle
        indices.push_back(index);
        indices.push_back(index + 1);
        indices.push_back(0);

        // The second triangle
        indices.push_back(index + 1);
        indices.push_back(1);
        indices.push_back(0);


        Mesh* mesh = new Mesh("road");
        mesh->InitFromData(vertices, indices);

        return mesh;
	}

}