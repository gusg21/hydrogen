#include "core/systems/sys_rendering.h"

#include "SDL3/SDL.h"
#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "core/math/mat4x4.h"
#include "core/model.h"
#include "imgui_impl_sdl3.h"

#define RENDERING_INIT_FAIL_UNABLE_TO_BGFX 1

uint32_t h_core::system::Rendering::init() {
    return 0;
}

uint32_t h_core::system::Rendering::initWindow(
    uint32_t width, uint32_t height, void* nwh) {
    bgfx::renderFrame();  // TODO: required? Docs say needed to indicate
                          // single-threaded rendering.

    bgfx::Init init;

    // Acquire native handle
    init.platformData.nwh = nwh;

    // Set up resolution + backbuffer settings
    init.resolution.width = width;
    init.resolution.height = height;
    init.resolution.reset = BGFX_RESET_VSYNC;

    if (!bgfx::init(init)) { return RENDERING_INIT_FAIL_UNABLE_TO_BGFX; }

    return 0;
}

void h_core::system::Rendering::process() {
    h_core::math::Mat4x4 view = h_core::math::Mat4x4::lookAtMat(cameraPosition, cameraDirection);

    //h_core::math::Mat4x4 proj = 
}

h_core::ComponentBitmask h_core::system::Rendering::getMask() {
    return TRANSFORM_COMPONENT_BITMASK & MODEL_COMPONENT_BITMASK;
}