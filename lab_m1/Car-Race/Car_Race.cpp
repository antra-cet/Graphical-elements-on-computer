#include "lab_m1/Car-Race/Car_Race.h"
#include "lab_m1/Car-Race/CarShapes.h"
#include "lab_m1/Car-Race/transform3D.h"


#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


Car_Race::Car_Race()
{
}


Car_Race::~Car_Race()
{
}


void Car_Race::Init()
{
    renderCameraTarget = false;

    // Initialize camera
    carCamera = new implemented::Camera();
    carCamera->Set(glm::vec3(CAR_CAMERA_START_X, CAR_CAMERA_START_Y, CAR_CAMERA_START_Z),
        glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    // car
    {
        Mesh* mesh = new Mesh("car");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "props"), "audi.mtl.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    
    // other cars
    {
        Mesh* mesh = new Mesh("enemy");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "props"), "BMW.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // road
    {
        Car_Shapes::CreateRoad(meshes);
    }

    // earth
    {
        Car_Shapes::CreateGrass(meshes);
    }

    // tree
    {
        Mesh* mesh = new Mesh("tree");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "props"), "tree.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // planet
    {
        Mesh* mesh = new Mesh("planet");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "props"), "venus.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("GalacticCarShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Car-Race", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Car-Race", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("GalacticCarShaderImports");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Car-Race", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Car-Race", "shaders", "FragmentShaderObjects.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // parameters, remove hardcodings of these parameters
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    // Initialize skel track points for the road
    skelRoadPoints = Car_Shapes::CreateTrackPoints();

    // Initialize track points for enemy cars
    moveEnemyCarPoints = Car_Shapes::CreateEnemyTrackPoints(skelRoadPoints);

    // Initialize the triangles of the road
    roadTriangles = Car_Shapes::CreateTrainglePoints(skelRoadPoints, ROAD_WIDTH);

    // Initialize the tree points
    trees = Car_Shapes::CreateTreePoints(roadTriangles);

    // Initialize enemy car points
    enemyCars = getEnemyCars(moveEnemyCarPoints);

    // Initialize the car's starting point
    car.angle = 0;
}


void Car_Race::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(1, 0.71, 0.45, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Car_Race::RenderScene()
{
    // car
    {
        car.translate = carCamera->GetTargetPosition();

        // Bring the car on the road
        car.translate.y -= 1.5;

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(car.translate.x, car.translate.y, car.translate.z);
        modelMatrix *= transform3D::Scale(CAR_SCALE, CAR_SCALE, CAR_SCALE);
        modelMatrix *= transform3D::RotateOY(car.angle);

        RenderMesh(meshes["car"], shaders["GalacticCarShaderImports"], modelMatrix);
    }

    // enemy cars
    {
        // Each enemy car will start at a random point on the track
        for (int i = 0; i < ENEMY_CARS_NUM; i++) {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(enemyCars[i].translate.x, enemyCars[i].translate.y, enemyCars[i].translate.z);
            modelMatrix *= transform3D::RotateOY(enemyCars[i].angle);

            RenderMesh(meshes["enemy"], shaders["GalacticCarShaderImports"], modelMatrix);
        }
    }

    // tree
    {
        for (int i = 0; i < trees.size(); i++) {
            glm::vec3 translateTree;
            translateTree.x = trees[i].x;
            translateTree.y = 0;
            translateTree.z = trees[i].z;

            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(translateTree.x, translateTree.y, translateTree.z);

            RenderMesh(meshes["tree"], shaders["GalacticCarShaderImports"], modelMatrix);
        }
    }

    // road
    {
        for (int i = 0; i < 1000; i++) {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(0, 0.001 * i, 0);

            RenderMesh(meshes["road"], shaders["GalacticCarShader"], modelMatrix);
        }
    }

    // grass
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(0, -0.1, 0);

        RenderMesh(meshes["grass"], shaders["GalacticCarShader"], modelMatrix);
    }

    // planet
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(0, 250, -300);
        modelMatrix *= transform3D::Scale(0.1, 0.1, 0.1);

        RenderMesh(meshes["planet"], shaders["VertexColor"], modelMatrix);
    }
}

void Car_Race::Update(float deltaTimeSeconds)
{

    glm::ivec2 resolution = window->GetResolution();

    // Draws map
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 400.0f);
    glViewport(0, 0, resolution.x, resolution.y);
    RenderScene();

    // Gets a copy of the car elements
    implemented::Camera auxCamera;
    auxCamera.forward = carCamera->forward;
    auxCamera.position = carCamera->position;
    auxCamera.up = carCamera->up;
    auxCamera.right = carCamera->right;
    auxCamera.distanceToTarget = carCamera->distanceToTarget;
    
    // Draws minimap
    car.translate = carCamera->GetTargetPosition();
    carCamera->Set(glm::vec3(car.translate.x, 50.0f, car.translate.z), glm::vec3(car.translate.x, 0, car.translate.z), glm::vec3(0, 1, 0));
    projectionMatrix = glm::ortho((float)((-1) * resolution.x / MINIMAP_PROP) / 2, (float)(resolution.x / MINIMAP_PROP) / 2,
        (float)((-1) * resolution.y / (4 * MINIMAP_PROP)) / 2, (float)(resolution.y / (4 * MINIMAP_PROP)) / 2, 0.01f, 400.0f);
    glViewport(MINIMAP_SIZE_X, MINIMAP_SIZE_Y, resolution.x / MINIMAP_PROP, resolution.y / MINIMAP_PROP);
    RenderScene();

    // Resets car camera
    carCamera->forward = auxCamera.forward;
    carCamera->position = auxCamera.position;
    carCamera->up = auxCamera.up;
    carCamera->right = auxCamera.right;
    carCamera->distanceToTarget = auxCamera.distanceToTarget;
}


void Car_Race::FrameEnd()
{
}


void Car_Race::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(carCamera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    int location = glGetUniformLocation(shader->program, "car_translate");
    glUniform3fv(location, 1, glm::value_ptr(carCamera->GetTargetPosition()));

    mesh->Render();
}

void Car_Race::OnInputUpdate(float deltaTime, int mods)
{
    float cameraRotation = 1.0f;

    // Move the enemy cars x position at a time
    for (int pos = 0; pos < 20; pos++) {
        for (int i = 0; i < ENEMY_CARS_NUM; i++) {
            // Calculate the point the enemy car is approaching to
            float approachingIndex = ((int)enemyCars[i].indexPoint + 1) % moveEnemyCarPoints.size();
            float nextApproachingIndex = ((int)approachingIndex + 1) % moveEnemyCarPoints.size();

            glm::vec3 approachingPoint = moveEnemyCarPoints[approachingIndex];
            glm::vec3 nextApproachingPoint = moveEnemyCarPoints[nextApproachingIndex];

            // Move twoards the next point of the road list
            enemyCars[i].indexPoint = approachingIndex;
            enemyCars[i].translate.x = approachingPoint.x;
            enemyCars[i].translate.z = approachingPoint.z;
            enemyCars[i].angle = atan2((nextApproachingPoint.z - approachingPoint.z),
                (nextApproachingPoint.x - approachingPoint.x));

            //implemented::Camera auxCamera = *carCamera;
            //glm::vec3 auxTarget = getActualTargetPosition(auxCamera.GetTargetPosition());
            //if ((auxTarget.x - enemyCars[i].translate.x) <= ENEMY_CAR_SPACING &&
            //    (auxTarget.z - enemyCars[i].translate.z) <= ENEMY_CAR_SPACING) {
            //    cameraSpeed -= 3 * CAR_SPEED_INC;
            //}
        }
    }

    // Translate forward
    if (window->KeyHold(GLFW_KEY_W)) {

        // Car increase speed
        if (cameraSpeed < CAR_MAX_SPEED) {
            cameraSpeed += CAR_SPEED_INC;
        }

        // Verify if still on track after movement
        implemented::Camera auxCamera = *carCamera;
        auxCamera.MoveForward(cameraSpeed * deltaTime);
        glm::vec3 auxTarget = getActualTargetPosition(auxCamera.GetTargetPosition());

        if (isOnTrack(auxTarget, roadTriangles)) {
            carCamera->MoveForward(cameraSpeed * deltaTime);
        }
        else {
            // If it ran off track, decrease the speed to 0
            cameraSpeed = 0;
        }
    } else {
        // Car decrease speed until stop
        if (cameraSpeed >= CAR_STOP) {
            cameraSpeed -= CAR_SPEED_INC;
        }

        // Verify if still on track after movement
        implemented::Camera auxCamera = *carCamera;
        auxCamera.MoveForward(cameraSpeed * deltaTime);
        glm::vec3 auxTarget = getActualTargetPosition(auxCamera.GetTargetPosition());

        if (isOnTrack(auxTarget, roadTriangles)) {
            carCamera->MoveForward(cameraSpeed * deltaTime);
        }
        else {
            // If it ran off track, decrease the speed to 0
            cameraSpeed = 0;
        }
    }

    // Translate backward
    if (window->KeyHold(GLFW_KEY_S)) {
        // Car decrease speed if stopped
        if (cameraSpeed >= (- 1) * CAR_MAX_SPEED) {
            cameraSpeed -= CAR_SPEED_INC;
        }

        // Verify if still on track after movement
        implemented::Camera auxCamera = *carCamera;
        auxCamera.MoveForward(cameraSpeed * deltaTime);
        glm::vec3 auxTarget = getActualTargetPosition(auxCamera.GetTargetPosition());

        if (isOnTrack(auxTarget, roadTriangles)) {
            carCamera->MoveForward(cameraSpeed * deltaTime);
        }
        else {
            // If it ran off track, decrease the speed to 0
            cameraSpeed = 0;
        }
    } else {
        // Car increase speed until stop
        if (cameraSpeed <= CAR_STOP) {
            cameraSpeed += CAR_SPEED_INC;
        }

        // Verify if still on track after movement
        implemented::Camera auxCamera = *carCamera;
        auxCamera.MoveForward(cameraSpeed * deltaTime);
        glm::vec3 auxTarget = getActualTargetPosition(auxCamera.GetTargetPosition());

        if (isOnTrack(auxTarget, roadTriangles)) {
            carCamera->MoveForward(cameraSpeed * deltaTime);
        }
        else {
            // If it ran off track, decrease the speed to 0
            cameraSpeed = 0;
        }
    }

    // Rotate car left
    if (window->KeyHold(GLFW_KEY_A)) {
        // Verify if still on track after movement
        implemented::Camera auxCamera = *carCamera;
        auxCamera.RotateThirdPerson_OY(cameraRotation * deltaTime);
        glm::vec3 auxTarget = getActualTargetPosition(auxCamera.GetTargetPosition());

        if (isOnTrack(auxTarget, roadTriangles)) {
            carCamera->RotateThirdPerson_OY(cameraRotation * deltaTime);

            // increase car rotation angle
            car.angle += cameraRotation * deltaTime;
        }
    }

    // Rotate car rights
    if (window->KeyHold(GLFW_KEY_D)) {
        // Verify if still on track after movement
        implemented::Camera auxCamera = *carCamera;
        auxCamera.RotateThirdPerson_OY(-cameraRotation * deltaTime);
        glm::vec3 auxTarget = getActualTargetPosition(auxCamera.GetTargetPosition());

        if (isOnTrack(auxTarget, roadTriangles)) {
            carCamera->RotateThirdPerson_OY(-cameraRotation * deltaTime);

            // decrease car rotation angle
            car.angle -= cameraRotation * deltaTime;
        }
    }

    // TODO : delete translate camera upward and downward
    if (window->KeyHold(GLFW_KEY_Q)) {
        carCamera->TranslateUpward(-10 * deltaTime);
    }
    
    if (window->KeyHold(GLFW_KEY_E)) {
        carCamera->TranslateUpward(10 * deltaTime);
    }
}

void Car_Race::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

        if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;
            carCamera->RotateFirstPerson_OY(-deltaX * sensivityOX);
            carCamera->RotateFirstPerson_OX(-deltaY * sensivityOY);
        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            renderCameraTarget = true;
            carCamera->RotateThirdPerson_OY(-deltaX * sensivityOX);
            carCamera->RotateThirdPerson_OX(-deltaY * sensivityOY);
        }
    }
}
