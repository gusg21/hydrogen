#include "core/runtimesystem.h"

#include "core/scene.h"

uint32_t h_core::RuntimeSystem::init(h_core::RuntimeEngine* engine) {
    this->engine = engine;

    return 0;
}