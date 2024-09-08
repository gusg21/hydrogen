#pragma once

#include "angelscript.h"

#include "core/math/math.h"
#include "core/system.h"
#include "core/systems/script/scriptcomp.h"

namespace h_core {
namespace script {

class Scripting : public System {
  public:
    uint32_t init(h_core::Engine* engine) override;
    void destroy() override;
    void initPerActor() override;
    void beginFrame() override;
    void process() override;
    void draw() override;
    void endFrame() override;

    [[nodiscard]] asIScriptContext* getContext() const;
    [[nodiscard]] asIScriptModule* getModule() const;

    h_core::Transform getBoundTransform();
    void setBoundTransform(h_core::Transform newTrans);

    HYSYSTEM(h_core::script::ScriptComp::getMask());

  private:
    asIScriptEngine* m_scriptEngine = nullptr;
    asIScriptModule* m_scriptModule = nullptr;
    asIScriptContext* m_scriptContext = nullptr;
};
}  // namespace script

}  // namespace h_core
