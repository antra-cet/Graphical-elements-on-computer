#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Car-Race/camera.h"
#include "lab_m1/Car-Race/CarUtils.h"
#include "components/transform.h"


namespace m1
{
    class Car_Race : public gfxc::SimpleScene
    {
    public:
        Car_Race();
        ~Car_Race();

        void Init() override;

    private:
        void FrameStart() override;
        void RenderScene();
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;

    protected:
        // Camera
        implemented::Camera* carCamera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        float cameraSpeed = 5.0f;

        // My car
        Car_Utils::car car;

        // The enemy cars
        std::vector<Car_Utils::car> enemyCars;
        std::vector<glm::vec3> moveEnemyCarPoints;

        // The surroundings
        std::vector<glm::vec3> skelRoadPoints;
        std::vector<glm::vec3> trees;
        std::vector<glm::vec3> roadTriangles;
    };
}   // namespace m1
