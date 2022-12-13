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
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        float fov;
        float width;
        float height;


        glm::vec3 lightPosition;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;


        // MINE
        implemented::Camera* carCamera, * miniMapCamera;

        Car_Utils::car car;
        std::vector<glm::vec3> trees;
        std::vector<glm::vec3> roadTriangles;
    };
}   // namespace m1
