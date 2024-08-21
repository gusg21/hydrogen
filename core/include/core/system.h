#pragma once

#include "core/actor.h"
#include "core/componentbitmask.h"
#include "core/transform.h"
#include "core/mesh.h"

namespace h_core {
//class Model;
class Engine;

class System {
  public:
    virtual uint32_t init() = 0;
    virtual void destroy() {};
    virtual void beginFrame() {};
    virtual void process() {};
    virtual void draw() {};
    virtual void endFrame() {};

    virtual h_core::ComponentBitmask getMask() = 0;

    h_core::ActorId actorId;
    h_core::Engine* engine;
    h_core::Transform* transform;
    h_core::Mesh* mesh;
};
}  // namespace h_core
