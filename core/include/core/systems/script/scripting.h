#pragma once

#include "angelscript.h"

#include "core/math/math.h"
#include "core/system.h"

namespace h_core {
namespace script {

class Scripting : public System {
  public:
    uint32_t init(h_core::Engine* engine);
    void destroy();
    void initPerActor();
    void beginFrame();
    void process();
    void draw();
    void endFrame();

    uint32_t getMask();
    asIScriptContext* getContext();
    asIScriptModule* getModule();

    h_core::Transform getBoundTransform();
    void setBoundTransform(h_core::Transform newTrans);

  private:
    asIScriptEngine* scriptEngine = nullptr;
    asIScriptModule* scriptModule = nullptr;
    asIScriptContext* scriptContext = nullptr;

    bool scriptModuleBuilt = true;
};
}  // namespace script

}  // namespace h_core
