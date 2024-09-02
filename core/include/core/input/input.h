//
// Created by Angus Goucher on 9/2/2024.
//

#pragma once

#include <vector>
#include <unordered_map>

#include "SDL.h"

#include "core/input/inputaction.h"
#include "core/project/project.h"

namespace h_core {
namespace input {
typedef uint32_t InputActionIndex;

class Input {
  public:
    void init(h_core::project::Project* project);
    void swapInputBuffers();

    float getAnalogValue(InputActionIndex actionIndex);
    bool getDigitalValue(InputActionIndex actionIndex, float deadzone = 0.2f);

    bool isKeyDown(SDL_Scancode key);

    const std::vector<h_core::input::InputAction*>* getActions();
    h_core::input::InputAction* getAction(h_core::input::InputActionIndex actionIndex);
    h_core::input::InputActionIndex getActionIndex(const std::string& actionName);
    h_core::input::InputActionIndex newAction(const std::string& name);

  private:
    std::vector<h_core::input::InputAction*> m_actions {};
    std::unordered_map<uint32_t, h_core::input::InputActionIndex>
        m_actionNameMap {};

    const uint8_t* m_currentKeys = nullptr;
    uint8_t* m_previousKeys = nullptr;
};
}
}