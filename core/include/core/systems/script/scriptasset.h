#pragma once

#include "angelscript.h"
#include "yaml-cpp/yaml.h"

#include <string>

#include "core/actor.h"
#include "core/asset.h"


#define SCRIPT_RUN_METHOD_FAIL_BAD_PREPARE  1
#define SCRIPT_RUN_METHOD_FAIL_BAD_INSTANCE 2
#define SCRIPT_RUN_METHOD_FAIL_BAD_EXECUTE  3

#define SCRIPT_COMPILE_FAIL_NO_DEFINED_TYPES 1
#define SCRIPT_COMPILE_FAIL_BAD_BUILD        2

namespace h_core {
namespace script {

class ScriptAsset : public Asset {
  public:
    ScriptAsset() = default;

    uint32_t initFromYaml(
        h_core::Assets* assets, h_core::Systems* system,
        YAML::Node node) override;
    uint32_t initFromFile(
        h_core::Assets* assets, h_core::Systems* systems,
        std::string filepath) override;


    uint32_t compile(asIScriptContext* context);
    asIScriptObject* constructInstance(
        asIScriptContext* context, h_core::ActorId id) const;

    std::string code {};
    std::string name {};

    asITypeInfo* type = nullptr;
    asIScriptFunction* typeConstructor = nullptr;
    asIScriptModule* module = nullptr;
};
}  // namespace script

}  // namespace h_core
