#include "lab_m1/Duck-Hunt/Duck-Hunt.h"
#include "lab_m1/Duck-Hunt/duckUtils.h"
#include "lab_m1/Duck-Hunt/duckShapes.h"
#include "lab_m1/Duck-Hunt/duckMovement.h"
#include "lab_m1/Duck-Hunt/transform2D.h"

#include <iostream>
#include <GL/glew.h>
#include <vector>
#include <math.h>
#include <random>

using namespace std;
using namespace m1;

void Duck_Hunt::Init()
{
    // Changing the resolution
    resolution = window->GetResolution();
    lastResolution = resolution;

    // Setting the camera
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // Initial duck variables
    duck.score = DUCK_INITIAL_SCORE;
    duck.lives = DUCK_INITIAL_LIVES;
    duck.speed = DUCK_INITIAL_SPEED;
    duck.movementCase = duckStructures::DUCK_FLY;
    duck.killingSpree = 0;

    // Set the other parameters
    duckUtils::duckSetParameters(duck, resolution);
    duckShapes::CreateMeshes(meshes, resolution);
}


void Duck_Hunt::FrameStart()
{
    resolution = window->GetResolution();

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.8, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Duck_Hunt::UpdateAlive(float deltaTimeSeconds) {
    // Draw the score
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(1000, 650);

    RenderMesh2D(meshes["score-wireframe"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(1000, 650);
    modelMatrix *= transform2D::Scale(duck.score, 1);

    RenderMesh2D(meshes["score"], shaders["VertexColor"], modelMatrix);

    // Draw number of lives
    for (int i = 0; i < duck.lives; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(30 + i * 30, 700);

        RenderMesh2D(meshes["heart"], shaders["VertexColor"], modelMatrix);
    }

    // Draw number of bullets
    for (int i = 0; i < duck.numBullets; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(25 + i * 30, 620);

        RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);
    }

    // Draw the streaks
    for (int i = 1; i < duck.killingSpree; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(500 + i * 40, 620);

        RenderMesh2D(meshes["fire-center"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["fire-inside"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["fire-background"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["fire-triangle"], shaders["VertexColor"], modelMatrix);
    }

    // Move duck
    duckMovement::moveDuck(duck, deltaTimeSeconds, resolution);

    // Draw the duck
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(duck.translateDuck.x, duck.translateDuck.y);
    modelMatrix *= transform2D::Rotate(duck.angle * twicePI / 360);

    RenderMesh2D(meshes["head"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["duck_body"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["beak"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(duck.wingMove.moveWing1.x + duck.translateDuck.x, duck.translateDuck.y);
    modelMatrix *= transform2D::Rotate(duck.angle * twicePI / 360);

    RenderMesh2D(meshes["duck_wing1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(duck.wingMove.moveWing2.x + duck.translateDuck.x, duck.translateDuck.y);
    modelMatrix *= transform2D::Rotate(duck.angle * twicePI / 360);

    RenderMesh2D(meshes["duck_wing2"], shaders["VertexColor"], modelMatrix);

    // Move the wings of the duck
    if (duck.wingMove.moveWing1.x <= LOWER_BOUND_WING_X) {
        duck.wingMove.moveWings = 1;
    } else if (duck.wingMove.moveWing1.x >= UPPER_BOUND_WING_X) {
        duck.wingMove.moveWings = -1;
    }

    duck.wingMove.moveWing1.x += deltaTimeSeconds * WING_SPEED * duck.wingMove.moveWings;
    duck.wingMove.moveWing2.x -= deltaTimeSeconds * WING_SPEED * duck.wingMove.moveWings;
}

void Duck_Hunt::Update(float deltaTimeSeconds)
{
    // Draw everything that should be in front of the duck
    modelMatrix = glm::mat3(1);
    RenderMesh2D(meshes["ground"], shaders["VertexColor"], modelMatrix);

    if (duck.score > MAX_SCORE) {
        // Create tick for winning

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(lastResolution.x / 2, LOWER_BOUND_Y + (lastResolution.y - LOWER_BOUND_Y) / 2);
        modelMatrix *= transform2D::Rotate(45 * twicePI / 360);

        RenderMesh2D(meshes["v-rectangle"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(lastResolution.x / 2, LOWER_BOUND_Y + (lastResolution.y - LOWER_BOUND_Y) / 2);
        modelMatrix *= transform2D::Rotate(135 * twicePI / 360);

        RenderMesh2D(meshes["v-rectangle"], shaders["VertexColor"], modelMatrix);
    }
    else {
        if (duck.lives > 0) {
            // While there are still lives, move the duck
            UpdateAlive(deltaTimeSeconds);
        }
        else {
            // Create the error for losing
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(lastResolution.x / 2, LOWER_BOUND_Y + (lastResolution.y - LOWER_BOUND_Y) / 2);
            modelMatrix *= transform2D::Rotate(45 * twicePI / 360);

            RenderMesh2D(meshes["x-rectangle"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(lastResolution.x / 2, LOWER_BOUND_Y + (lastResolution.y - LOWER_BOUND_Y) / 2);
            modelMatrix *= transform2D::Rotate((-45) * twicePI / 360);

            RenderMesh2D(meshes["x-rectangle"], shaders["VertexColor"], modelMatrix);
        }
    }
}

void Duck_Hunt::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // If the duck was moved, then change parameters
    if (duck.movementCase == duckStructures::DUCK_FLY) {
        if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
            if (mouseX >= duck.translateDuck.x - 200 && mouseY >= duck.translateDuck.y - 200 &&
                mouseX <= duck.translateDuck.x + 300 && mouseY <= duck.translateDuck.y + 300 &&
                duck.numBullets > 0) {

                // If the duck was hit and the player still has bullets left, then
                // change the numeber of bullets and the movement case in which
                // the duck is
                duck.movementCase = duckStructures::DUCK_HIT;
                duck.numBullets--;

                // Increase the killing streak until 4
                if (duck.killingSpree < 4) {
                    duck.killingSpree++;
                }
            }
            else {
                // If the duck button was pressed and the duck wasn't hit,
                // decrease the number of bullets
                duck.numBullets--;
            }
        }
    }
}

/*
    Unused functions
*/

Duck_Hunt::Duck_Hunt()
{
}


Duck_Hunt::~Duck_Hunt()
{
}

void Duck_Hunt::FrameEnd()
{
}

void Duck_Hunt::OnInputUpdate(float deltaTime, int mods)
{
}

void Duck_Hunt::OnKeyPress(int key, int mods)
{
}


void Duck_Hunt::OnKeyRelease(int key, int mods)
{
}

void Duck_Hunt::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Duck_Hunt::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Duck_Hunt::OnWindowResize(int width, int height)
{
}

void Duck_Hunt::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}