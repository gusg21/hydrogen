#pragma once

#include "core/actor.h"
#include "core/componentbitmask.h"
#include "core/transform.h"
#include "core/systems/render/mesh.h"
#include "core/systems/script/scriptcomp.h"

namespace h_core {
//class Model;
class Engine;

class System {
  public:
    virtual uint32_t init(h_core::Engine* engine) = 0;
    virtual void destroy() {};
    virtual void initPerActor() {};
    virtual void beginFrame() {};
    virtual void process() {};
    virtual void draw() {};
    virtual void endFrame() {};

    virtual h_core::ComponentBitmask getMask() = 0;

    h_core::ActorId actorId = 0;
    h_core::Engine* engine = nullptr;
    h_core::Transform* transform = nullptr;
    h_core::render::Mesh* mesh = nullptr;
    h_core::script::ScriptComp* script = nullptr;
};
}  // namespace h_core
