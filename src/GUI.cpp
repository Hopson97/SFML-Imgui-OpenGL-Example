#include "GUI.h"

#include <SFML/System/Clock.h>
#include <imgui/imgui.h>
#include <imgui_impl/imgui_wrapper.h>

#include "Camera.h"

sf::Clock fpsClock;
int frames = 0;
int fps;

void guiInit(sf::Window& window)
{
    ImGui_SfGl::init(window);

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 2;
    style.FrameRounding = 0;
    style.PopupRounding = 0;
    style.ScrollbarRounding = 0;
    style.TabRounding = 6;
}

void guiShutdown()
{
    ImGui_SfGl::shutdown();
}

void guiBeginFrame()
{
    ImGui_SfGl::startFrame();
    frames++;
    if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
        fps = frames;
        fpsClock.restart();
        frames = 0;
    }
}

void guiProcessEvent(sf::Event& event)
{
    ImGui_ImplSfml_ProcessEvent(event);
}

void guiDebugScreen(const Camera& c)
{
    if (ImGui::Begin("Debug")) {
        ImGui::Text("Player Position: (%f %f %f)", c.position.x, c.position.y, c.position.z);
        ImGui::Text("Player Rotation: (%f %f %f)", c.rotation.x, c.rotation.y, c.rotation.z);
        ImGui::Text("FPS: %d ", fps);
    }
    ImGui::End();
}

void guiEndFrame()
{
    ImGui_SfGl::endFrame();
}
