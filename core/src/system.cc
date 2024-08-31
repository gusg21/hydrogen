#include "core/system.h"

#include "core/scene.h"

uint32_t h_core::System::init(h_core::Engine* engine) {
    this->engine = engine;

    return 0;
}