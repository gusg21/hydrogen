#pragma once

#include "angelscript.h"

#include "core/runtimesystem.h"
#include "scriptcomp.h"

namespace h_core {
namespace script {

class Scripting : public RuntimeSystem {
  public:
    uint32_t init(h_core::RuntimeEngine* engine) override;
    void destroy() override;
    void initPerActor() override;
    void beginFrame() override;
    void process() override;
    void draw() override;
    void endFrame() override;

    [[nodiscard]] asIScriptContext* getContext() const;
    [[nodiscard]] asIScriptModule* getModule() const;

    float getDistanceToCamera();

    h_core::Transform getBoundTransform();
    h_core::AssetIndex getBoundModel();
    void setBoundTransform(h_core::Transform newTrans);


    HYSYSTEM(h_core::script::ScriptComp::getMask());

  private:
    asIScriptEngine* m_scriptEngine = nullptr;
    asIScriptModule* m_scriptModule = nullptr;
    asIScriptContext* m_scriptContext = nullptr;
};
}  // namespace script

}  // namespace h_core
