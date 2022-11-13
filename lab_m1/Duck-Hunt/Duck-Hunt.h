#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Duck-Hunt/duckStructures.h"

#include <vector>
#include <iostream>
#include <math.h>

namespace m1
{
    class Duck_Hunt : public gfxc::SimpleScene
    {
     public:
        Duck_Hunt();
        ~Duck_Hunt();

        void Init() override;

     private:
        void FrameStart() override;
        void UpdateAlive(float deltaTimeSeconds);
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        // translation matrix
        glm::mat3 modelMatrix;

        // duck movement
        duckStructures::duckUtils duck;

        // the resolution of the window
        glm::ivec2 resolution;
    };
}   // namespace m1
