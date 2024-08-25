#pragma once

#include "core/actor.h"
#include "core/componentbitmask.h"
#include "core/transform.h"
#include "core/systems/render/mesh.h"
#include "core/systems/script/script.h"

namespace h_core {
//class Model;
class Engine;

class System {
  public:
    virtual uint32_t init() = 0;
    virtual void destroy() {};
    virtual void initPerActor() {};
    virtual void beginFrame() {};
    virtual void process() {};
    virtual void draw() {};
    virtual void endFrame() {};

    virtual h_core::ComponentBitmask getMask() = 0;

    h_core::ActorId actorId;
    h_core::Engine* engine;
    h_core::Transform* transform;
    h_core::render::Mesh* mesh;
    h_core::script::Script* script;
};
}  // namespace h_core
