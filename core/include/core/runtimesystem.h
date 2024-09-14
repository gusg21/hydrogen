#pragma once

#include "core/actor.h"
#include "core/componentbitmask.h"

#define HYSYSTEM(mask) [[nodiscard]] constexpr static h_core::ComponentBitmask getMask() { return mask; }

namespace h_core {
namespace script {
class ScriptComp;
}
namespace render {
class MeshComp;
}
namespace physics {
class Rigidbody;
}

// class Model;
class RuntimeEngine;
class Transform;

class RuntimeSystem {
  public:
    virtual uint32_t init(h_core::RuntimeEngine* engine);
    virtual void destroy() {};
    virtual void initPerActor() {};
    virtual void doGUI() {};
    virtual void beginFrame() {};
    virtual void process() {};
    virtual void draw() {};
    virtual void endFrame() {};

    HYSYSTEM(0);

    h_core::ActorId actorId = 0;
    h_core::RuntimeEngine* engine = nullptr;
    h_core::Transform* transform = nullptr;
    h_core::render::MeshComp* meshComp = nullptr;
    h_core::script::ScriptComp* script = nullptr;
    h_core::physics::Rigidbody* rigidbody = nullptr;
};
}  // namespace h_core
