#include "core/sprinkles/testsystem.h"

#include "stdio.h"

#include "core/scene.h"

// NOTE: This system is a dummy! Useful for reference and testing but probably
// not in any other cases.

uint32_t h_core::sprinkles::TestSystem::init(h_core::RuntimeEngine* engine) {
    h_core::RuntimeSystem::init(engine);

    return 0;
}

void h_core::sprinkles::TestSystem::process() {

}