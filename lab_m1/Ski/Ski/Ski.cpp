#include "lab_m1/Ski/Ski.h"
#include "lab_m1/Ski/Ski_Utils.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace m1;

#define SLOPE_ANGLE 20.0f

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Ski::Ski() {}

Ski::~Ski() {}

void Ski::Init() {

    // Initialize camera
    camera = new skiCamera::Camera();
    camera->Set(GetSceneCamera()->m_transform->GetWorldPosition(), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    projectionMatrix = GetSceneCamera()->GetProjectionMatrix();

    // Textures
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Ski", "textures");

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "snow.jpeg").c_str(), GL_REPEAT);
        mapTextures["snow"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "red.jpg").c_str(), GL_REPEAT);
        mapTextures["red"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "ski.jpeg").c_str(), GL_REPEAT);
        mapTextures["skier"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "rock.jpg").c_str(), GL_REPEAT);
        mapTextures["rock"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "concrete.jpg").c_str(), GL_REPEAT);
        mapTextures["concrete"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "wood.jpeg").c_str(), GL_REPEAT);
        mapTextures["wood"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "grass_bilboard.png").c_str(), GL_REPEAT);
        mapTextures["grass"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "gift.jpeg").c_str(), GL_REPEAT);
        mapTextures["gift"] = texture;
    }

    // Meshes
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(
            PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"),
            "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(
            PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"),
            "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("cone");
        mesh->LoadMesh(
            PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"),
            "cone.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives", ""), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the
    // normal
    {
        Shader* shader = new Shader("TextureShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Ski", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Ski", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("MovingTextureShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Ski", "shaders", "VertexShaderTexture.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Ski", "shaders", "FragmentShaderTextures.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Skier and plane initialize
    {
        skier.inSpacePosition = glm::vec3(0, 0, 0);
        slopeTranslate = glm::vec3(0, 0, 0);
        
        skier.speed = 10;
        skier.angle = 0;
        skier.hit = 0;
        skier.score = 0;

        endColor = glm::vec4(0, 0, 0, 1);
    }

    // Initialize the random objects vectors
    {
        for (int i = 0; i < MAX_ROCKS; i++) {
            Ski_Utils::randObjects rock;
            rock.render = 0;
            rock.touched = 0;
            rocks.push_back(rock);
        }

        for (int i = 0; i < MAX_GIFTS; i++) {
            Ski_Utils::randObjects gift;
            gift.render = 0;
            gift.touched = 0;
            gifts.push_back(gift);
        }

        for (int i = 0; i < MAX_POLES; i++) {
            Ski_Utils::randObjects pole;
            pole.render = 0;
            pole.touched = 0;
            poles.push_back(pole);
        }

        for (int i = 0; i < MAX_TREES; i++) {
            Ski_Utils::randObjects tree;
            tree.render = 0;
            tree.touched = 0;
            trees.push_back(tree);
        }
    }
}

void Ski::FrameStart() {
    // Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(endColor[0], endColor[1], endColor[2], endColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Ski::Update(float deltaTimeSeconds) {

    if (skier.hit == 1) {
        endColor = glm::vec4(0, 0, 0, 1);
        
        cout << "Score: " << skier.score << endl;
    }
    else {
        // Render slope
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, slopeTranslate);
            modelMatrix = glm::rotate(modelMatrix, RADIANS(SLOPE_ANGLE), glm::vec3(1, 0, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f));

            RenderSimpleMeshTexture(meshes["plane"], shaders["MovingTextureShader"], modelMatrix, mapTextures["snow"]);
        }

        // Render skis
        {
            glm::vec3 skiTranslate;
            skiTranslate = skier.inSpacePosition;

            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(skiTranslate.x - 0.07, skiTranslate.y - 0.1, skiTranslate.z - 0.1));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(SLOPE_ANGLE), glm::vec3(1, 0, 0));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(skier.angle), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f, 0.005f, 0.7f));

            RenderSimpleMeshTexture(meshes["box"], shaders["TextureShader"], modelMatrix, mapTextures["red"]);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(skiTranslate.x + 0.07, skiTranslate.y - 0.1, skiTranslate.z - 0.1));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(SLOPE_ANGLE), glm::vec3(1, 0, 0));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(skier.angle), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f, 0.005f, 0.7f));

            RenderSimpleMeshTexture(meshes["box"], shaders["TextureShader"], modelMatrix, mapTextures["red"]);
        }

        // Render skier
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, skier.inSpacePosition);
            modelMatrix = glm::rotate(modelMatrix, RADIANS(SLOPE_ANGLE), glm::vec3(1, 0, 0));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(skier.angle), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));

            RenderSimpleMeshTexture(meshes["box"], shaders["TextureShader"], modelMatrix, mapTextures["skier"]);
        }

        // Render rocks
        {
            // Rendering rocks in random position within the seeable plane
            for (int i = 0; i < MAX_ROCKS; i++) {
                // Find the rocks that are not renedered anymore and replace them with other rocks
                if (rocks[i].render == 0) {
                    int sgn = (rand() % 2 == 0) ? 1 : -1;
                    float pos = 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3 - 1)));
                    float posX = sgn * (1 + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (4 - 1)));

                    rocks[i].inSpacePosition = glm::vec3(camera->GetTargetPosition().x + posX,
                        camera->GetTargetPosition().y - pos, camera->GetTargetPosition().z + 2 * pos);
                    rocks[i].render = 1;
                }

                // Choosing the position in the plane
                glm::vec3 rockTranslate = rocks[i].inSpacePosition;

                // Rendering the rock
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, rockTranslate);
                modelMatrix = glm::rotate(modelMatrix, RADIANS(SLOPE_ANGLE), glm::vec3(1, 0, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));

                RenderSimpleMeshTexture(meshes["sphere"], shaders["TextureShader"], modelMatrix, mapTextures["rock"]);

                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(rockTranslate.x + 0.05f, rockTranslate.y, rockTranslate.z));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(SLOPE_ANGLE), glm::vec3(1, 0, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));

                RenderSimpleMeshTexture(meshes["sphere"], shaders["TextureShader"], modelMatrix, mapTextures["rock"]);

                // Checking if the rock's position is still in the plane
                if (rockTranslate.y > camera->GetTargetPosition().y + 1.5f) {
                    rocks[i].render = 0;
                }

                // Checking if the skier touched the rock
                if (rockTranslate.y > skier.inSpacePosition.y - 0.2f &&
                    rockTranslate.y < skier.inSpacePosition.y + 0.2f &&
                    rockTranslate.x > skier.inSpacePosition.x - 0.2f &&
                    rockTranslate.x < skier.inSpacePosition.x + 0.2f &&
                    rockTranslate.z > skier.inSpacePosition.z - 0.2f &&
                    rockTranslate.z < skier.inSpacePosition.z + 0.2f) {
                    rocks[i].render = 0;
                    skier.hit = 1;
                }
            }
        }

        // Render lighting pole
        {
            // Rendering rocks in random position within the seeable plane
            for (int i = 0; i < MAX_POLES; i++) {
                // Find the rocks that are not renedered anymore and replace them with other rocks
                if (poles[i].render == 0) {
                    int sgn = (rand() % 2 == 0) ? 1 : -1;
                    float pos = 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3 - 1)));
                    float posX = sgn * (1 + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (4 - 1)));
                    poles[i].inSpacePosition = glm::vec3(camera->GetTargetPosition().x + posX,
                        camera->GetTargetPosition().y - pos, camera->GetTargetPosition().z + 2 * pos);
                    poles[i].render = 1;
                }

                // Choosing the position in the plane
                glm::vec3 poleTranslate = poles[i].inSpacePosition;

                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, poleTranslate);
                modelMatrix = glm::rotate(modelMatrix, RADIANS(SLOPE_ANGLE), glm::vec3(1, 0, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 1.1, 0.05));

                RenderSimpleMeshTexture(meshes["box"], shaders["TextureShader"], modelMatrix, mapTextures["concrete"]);

                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(poleTranslate.x, poleTranslate.y + 0.4, poleTranslate.z + 0.2));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(SLOPE_ANGLE), glm::vec3(1, 0, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.1, 0.1));

                RenderSimpleMeshTexture(meshes["box"], shaders["TextureShader"], modelMatrix, mapTextures["concrete"]);

                // Checking if the tree's position is still in the plane
                if (poleTranslate.y > camera->GetTargetPosition().y + 1.5f) {
                    poles[i].render = 0;
                }

                // Checking if the skier touched the rock
                if (poleTranslate.y > skier.inSpacePosition.y - 0.2f &&
                    poleTranslate.y < skier.inSpacePosition.y + 0.2f &&
                    poleTranslate.x > skier.inSpacePosition.x - 0.2f &&
                    poleTranslate.x < skier.inSpacePosition.x + 0.2f &&
                    poleTranslate.z > skier.inSpacePosition.z - 0.2f &&
                    poleTranslate.z < skier.inSpacePosition.z + 0.2f) {
                    poles[i].render = 0;
                    skier.hit = 1;
                }
            }
        }

        // Render tree
        {
            // Rendering rocks in random position within the seeable plane
            for (int i = 0; i < MAX_TREES; i++) {
                // Find the rocks that are not renedered anymore and replace them with other rocks
                if (trees[i].render == 0) {
                    int sgn = (rand() % 2 == 0) ? 1 : -1;
                    float pos = 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3 - 1)));
                    float posX = sgn * (1 + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (4 - 1)));

                    trees[i].inSpacePosition = glm::vec3(camera->GetTargetPosition().x + posX,
                        camera->GetTargetPosition().y - pos, camera->GetTargetPosition().z + 2 * pos);
                    trees[i].render = 1;
                }

                glm::vec3 treeTranslate = trees[i].inSpacePosition;

                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, treeTranslate);
                modelMatrix = glm::rotate(modelMatrix, RADIANS(SLOPE_ANGLE), glm::vec3(1, 0, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.3, 0.1));

                RenderSimpleMeshTexture(meshes["box"], shaders["TextureShader"], modelMatrix, mapTextures["wood"]);

                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(treeTranslate.x, treeTranslate.y + 0.4, treeTranslate.z + 0.2));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(SLOPE_ANGLE), glm::vec3(1, 0, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));

                RenderSimpleMeshTexture(meshes["cone"], shaders["TextureShader"], modelMatrix, mapTextures["grass"]);

                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(treeTranslate.x, treeTranslate.y + 0.5, treeTranslate.z + 0.2));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(SLOPE_ANGLE), glm::vec3(1, 0, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));

                RenderSimpleMeshTexture(meshes["cone"], shaders["TextureShader"], modelMatrix, mapTextures["grass"]);

                // Checking if the tree's position is still in the plane
                if (treeTranslate.y > camera->GetTargetPosition().y + 1.5f) {
                    trees[i].render = 0;
                }
                
                // Checking if the skier touched the rock
                if (treeTranslate.y > skier.inSpacePosition.y - 0.2f &&
                    treeTranslate.y < skier.inSpacePosition.y + 0.2f &&
                    treeTranslate.x > skier.inSpacePosition.x - 0.2f &&
                    treeTranslate.x < skier.inSpacePosition.x + 0.2f &&
                    treeTranslate.z > skier.inSpacePosition.z - 0.2f &&
                    treeTranslate.z < skier.inSpacePosition.z + 0.2f) {
                    trees[i].render = 0;
                    skier.hit = 1;
                }
                
            }
        }

        // Render gifts
        {
            for (int i = 0; i < MAX_GIFTS; i++) {
                // Find the rocks that are not renedered anymore and replace them with other rocks
                if (gifts[i].render == 0) {
                    int sgn = (rand() % 2 == 0) ? 1 : -1;
                    float pos = 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3 - 1)));
                    float posX = sgn * (1 + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (4 - 1)));

                    gifts[i].inSpacePosition = glm::vec3(camera->GetTargetPosition().x + posX,
                        camera->GetTargetPosition().y - pos, camera->GetTargetPosition().z + 2 * pos);
                    gifts[i].render = 1;
                }

                // Choosing the position in the plane
                glm::vec3 giftPosition = gifts[i].inSpacePosition;

                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, giftPosition);
                modelMatrix = glm::rotate(modelMatrix, RADIANS(SLOPE_ANGLE), glm::vec3(1, 0, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));

                RenderSimpleMeshTexture(meshes["box"], shaders["TextureShader"], modelMatrix, mapTextures["gift"]);

                // Checking if the gifts's position is still in the plane
                if (giftPosition.y > camera->GetTargetPosition().y + 1.5f) {
                    gifts[i].render = 0;
                }

                // Checking if the gift touched the player
				if (giftPosition.y > skier.inSpacePosition.y - 0.2f &&
					giftPosition.y < skier.inSpacePosition.y + 0.2f &&
					giftPosition.x > skier.inSpacePosition.x - 0.2f &&
					giftPosition.x < skier.inSpacePosition.x + 0.2f &&
					giftPosition.z > skier.inSpacePosition.z - 0.2f &&
					giftPosition.z < skier.inSpacePosition.z + 0.2f) {
					gifts[i].render = 0;
					skier.score += 1;
				}
            }
        }
    }
}

void Ski::FrameEnd() { }


void Ski::RenderSimpleMeshTexture(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1, Texture2D* texture2)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    int loc_texture_coord = glGetUniformLocation(shader->program, "text_coords");
    glUniform2f(loc_texture_coord, camera->GetTargetPosition().x, camera->GetTargetPosition().y);
    
    if (texture1)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->GetProgramID(), "texture_1"), 0);
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Ski::OnInputUpdate(float deltaTime, int mods) {
    // Moving the plane and camera and skier 
    camera->TranslateUpward(0.0007);
    slopeTranslate = camera->GetTargetPosition();
    for (int i = 0; i < MAX_ROCKS; i++) {
        rocks[i].inSpacePosition.y += 0.006;
        rocks[i].inSpacePosition.z -= 0.009;
    }

    for (int i = 0; i < MAX_GIFTS; i++) {
        gifts[i].inSpacePosition.y += 0.006;
        gifts[i].inSpacePosition.z -= 0.009;
    }

    for (int i = 0; i < MAX_POLES; i++) {
        poles[i].inSpacePosition.y += 0.006;
        poles[i].inSpacePosition.z -= 0.009;
    }

    for (int i = 0; i < MAX_TREES; i++) {
        trees[i].inSpacePosition.y += 0.006;
        trees[i].inSpacePosition.z -= 0.009;
    }

    if (skier.angle < 0) {
        camera->TranslateRight((-1) * 0.001);
        skier.inSpacePosition = camera->GetTargetPosition();
        skier.inSpacePosition.y += 0.5;
        skier.inSpacePosition.z -= 0.5;

        for (int i = 0; i < MAX_ROCKS; i++) {
            rocks[i].inSpacePosition.x += 0.015;
        }

        for (int i = 0; i < MAX_GIFTS; i++) {
            gifts[i].inSpacePosition.x += 0.015;
        }

        for (int i = 0; i < MAX_POLES; i++) {
            poles[i].inSpacePosition.x += 0.015;
        }

        for (int i = 0; i < MAX_TREES; i++) {
            trees[i].inSpacePosition.x += 0.015;
        }
    }
    else {
        camera->TranslateRight(0.001);
        skier.inSpacePosition = camera->GetTargetPosition();
        skier.inSpacePosition.y += 0.5;
        skier.inSpacePosition.z -= 0.5;

        for (int i = 0; i < MAX_ROCKS; i++) {
            rocks[i].inSpacePosition.x -= 0.015;
        }

        for (int i = 0; i < MAX_GIFTS; i++) {
            gifts[i].inSpacePosition.x -= 0.015;
        }

        for (int i = 0; i < MAX_POLES; i++) {
            poles[i].inSpacePosition.x -= 0.015;
        }

        for (int i = 0; i < MAX_TREES; i++) {
            trees[i].inSpacePosition.x -= 0.015;
        }
    }
}

void Ski::OnKeyPress(int key, int mods) {
    // Add key press event
}

void Ski::OnKeyRelease(int key, int mods) {
    // Add key release event
}

void Ski::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    // Add movement of the ski if the mouse moves 
    if (mouseX * deltaX < skier.inSpacePosition.x) {
        if (skier.angle > -45) {
            skier.angle -= skier.speed * 0.2;
        }
    }
    else {
        if (mouseX * deltaX > skier.inSpacePosition.x) {
            if (skier.angle < 45) {
                skier.angle += skier.speed * 0.2;
            }
        }
    }
}

void Ski::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
    // Add mouse button press event
}

void Ski::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
    // Add mouse button release event
}

void Ski::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}

void Ski::OnWindowResize(int width, int height) {}