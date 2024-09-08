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
// class Model;
class Engine;
class Transform;

class System {
  public:
    virtual uint32_t init(h_core::Engine* engine);
    virtual void destroy() {};
    virtual void initPerActor() {};
    virtual void doGUI() {};
    virtual void beginFrame() {};
    virtual void process() {};
    virtual void draw() {};
    virtual void endFrame() {};

    HYSYSTEM(0);

    h_core::ActorId actorId = 0;
    h_core::Engine* engine = nullptr;
    h_core::Transform* transform = nullptr;
    h_core::render::MeshComp* meshComp = nullptr;
    h_core::script::ScriptComp* script = nullptr;
};
}  // namespace h_core
