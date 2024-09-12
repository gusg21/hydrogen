//
// Created by Angus Goucher on 9/2/2024.
//

#pragma once

#include <vector>
#include <unordered_map>

#include "SDL.h"

#include "core/input/inputaction.h"
#include "core/project/project.h"
#include "core/window.h"

namespace h_core {
namespace input {
typedef uint32_t InputActionIndex;

class Input {
  public:
    Input() = default;

    void init(const h_core::project::Project* project, h_core::Window* window);
    void doGUI();
    void updateInternals();
    void setMousePos(uint32_t x, uint32_t y);
    void setMouseDelta(int32_t x, int32_t y);

    float getAnalogValue(InputActionIndex actionIndex);
    bool getDigitalValue(InputActionIndex actionIndex, float deadzone = 0.2f);
    bool getDigitalPressed(InputActionIndex actionIndex, float deadzone = 0.2f);
    bool getDigitalReleased(InputActionIndex actionIndex, float deadzone = 0.2f);

    bool isKeyDown(SDL_Scancode key);
    float getMouseX();
    float getMouseY();
    float getMouseDeltaX();
    float getMouseDeltaY();


    const std::vector<h_core::input::InputAction*>* getActions();
    h_core::input::InputAction* getAction(h_core::input::InputActionIndex actionIndex);
    h_core::input::InputActionIndex getActionIndex(const std::string& actionName);
    h_core::input::InputActionIndex newAction(const std::string& name);

    bool mouseCaptured = false;

  private:
    h_core::Window* m_window = nullptr;

    std::vector<h_core::input::InputAction*> m_actions {};
    std::unordered_map<uint32_t, h_core::input::InputActionIndex>
        m_actionNameMap {};

    const uint8_t* m_currentKeys = nullptr;
    uint8_t* m_previousKeys = nullptr;
    h_core::math::Vector2 m_mousePos {};
    h_core::math::Vector2 m_mouseDelta {};
};
}
}