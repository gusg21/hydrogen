#pragma once

#include "angelscript.h"

#include "core/system.h"

namespace h_core {
namespace script {

class Scripting : public System {
  public:
    uint32_t init();
    void destroy();
    void initPerActor();
    void beginFrame();
    void process();
    void draw();
    void endFrame();

    uint32_t getMask();

  private:
    asIScriptEngine* scriptEngine = nullptr;
    asIScriptModule* scriptModule = nullptr;

    bool scriptModuleBuilt = true;
      
};
}  // namespace script

}  // namespace h_core
