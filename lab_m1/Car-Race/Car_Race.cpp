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

    camera = new implemented::Camera();
    camera->Set(glm::vec3(0, 3, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    // car
    {
        Mesh* mesh = new Mesh("car");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "props"), "audi.mtl.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // sphere
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // road
    {
        Mesh *mesh = carShapes::CreateRoad();
        meshes[mesh->GetMeshID()] = mesh;
    }


    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("CarShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Car-Race", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Car-Race", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Light & material properties
    {
        lightPosition = glm::vec3(0, 1, 1);
        car.materialShininess = 30;
        car.materialKd = 0.5;
        car.materialKs = 0.5;
    }

    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    fov = RADIANS(60);
    width = 10;

    // Initialize my variables

    // Initialize the car's starting point
    car.angle = 0;
}


void Car_Race::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.62, 0.92, 0.9, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Car_Race::Update(float deltaTimeSeconds)
{
    // car
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        glm::vec3 cameraPosition;
        cameraPosition.x = camera->GetTargetPosition().x;
        cameraPosition.y = camera->GetTargetPosition().y;
        cameraPosition.z = camera->GetTargetPosition().z + CAR_START_POINT_Z;

        modelMatrix *= transform3D::Translate(camera->GetTargetPosition().x, camera->GetTargetPosition().y - CAR_START_POINT_Y, camera->GetTargetPosition().z - CAR_START_POINT_Z);
        modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
        modelMatrix *= transform3D::RotateOY(car.angle);
        RenderMesh(meshes["car"], shaders["VertexNormal"], modelMatrix);
    }

    // road
    {
        glm::mat4 modelMatrix = glm::mat4(1);

        RenderMesh(meshes["road"], shaders["Color"], modelMatrix);
    }

    // sun
    //{
    //    glm::mat4 modelMatrix = glm::mat4(1);
    //    modelMatrix = glm::scale(modelMatrix, glm::vec3(20, 20, 20));
    //    modelMatrix = glm::translate(modelMatrix, glm::vec3(lightPosition.x, lightPosition.y, lightPosition.z));

    //    RenderMesh(meshes["sphere"], shaders["Simple"], modelMatrix);
    //}
}


void Car_Race::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Car_Race::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Car_Race::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 10.0f;
    float cameraRotation = 1.0f;

    // Translate forward
    if (window->KeyHold(GLFW_KEY_W)) {
        camera->MoveForward(cameraSpeed * deltaTime);
    }

    // Translate backward
    if (window->KeyHold(GLFW_KEY_S)) {
        camera->MoveForward(-cameraSpeed * deltaTime);
    }

    // Translate left
    if (window->KeyHold(GLFW_KEY_A)) {
        camera->RotateThirdPerson_OY(cameraRotation * deltaTime);

        // Rotate car left
        car.angle += cameraRotation * deltaTime;
    }

    // Translate right
    if (window->KeyHold(GLFW_KEY_D)) {
        camera->RotateThirdPerson_OY(-cameraRotation * deltaTime);

        // Rotate car right
        car.angle -= cameraRotation * deltaTime;
    }


    // TODO : delete translate camera upward and downward
    if (window->KeyHold(GLFW_KEY_Q)) {
        camera->TranslateUpward(-cameraSpeed * deltaTime);
    }
    
    if (window->KeyHold(GLFW_KEY_E)) {
        camera->TranslateUpward(cameraSpeed * deltaTime);
    }
}


void Car_Race::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Car_Race::OnKeyRelease(int key, int mods)
{
    // Add key release event
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
            camera->RotateFirstPerson_OY(-deltaX * sensivityOX);
            camera->RotateFirstPerson_OX(-deltaY * sensivityOY);
        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            renderCameraTarget = true;
            camera->RotateThirdPerson_OY(-deltaX * sensivityOX);
            camera->RotateThirdPerson_OX(-deltaY * sensivityOY);
        }
    }
}


void Car_Race::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Car_Race::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Car_Race::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Car_Race::OnWindowResize(int width, int height)
{
}
