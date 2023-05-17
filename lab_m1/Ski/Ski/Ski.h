#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/Ski/Ski_Utils.h"
#include "lab_m1/Ski/camera.h"

#define MAX_ROCKS 5
#define MAX_GIFTS 2
#define MAX_POLES 5
#define MAX_TREES 5

namespace m1
{
    class Ski : public gfxc::SimpleScene
    {
    public:
        Ski();
        ~Ski();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMeshTexture(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1 = NULL, Texture2D* texture2 = NULL);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        skiCamera::Camera *camera;
        glm::mat4 projectionMatrix;
        
        std::unordered_map<std::string, Texture2D*> mapTextures;
        Ski_Utils::skier skier;
        int direction;
        glm::vec3 slopeTranslate;
        
        glm::vec4 endColor;

        std::vector<Ski_Utils::randObjects> rocks;
        std::vector<Ski_Utils::randObjects> gifts;
        std::vector<Ski_Utils::randObjects> trees;
        std::vector<Ski_Utils::randObjects> poles;
    };
}   // namespace m1
