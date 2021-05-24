#pragma once

#include <SFML/Window.hpp>

struct Camera;

void guiInit(sf::Window& window);
void guiShutdown();

void guiBeginFrame();
void guiProcessEvent(sf::Event& event);
void guiEndFrame();

void guiDebugScreen(const Camera& camera);
