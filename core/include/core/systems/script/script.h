#pragma once

#include "yaml-cpp/yaml.h"
#include "angelscript.h"

#include <string>

#include "core/asset.h"

#define SCRIPT_COMPONENT_BITMASK (1 << 2)

#define SCRIPT_RUN_METHOD_FAIL_BAD_PREPARE 1
#define SCRIPT_RUN_METHOD_FAIL_BAD_INSTANCE 2
#define SCRIPT_RUN_METHOD_FAIL_BAD_EXECUTE 3

namespace h_core {
namespace script {

class Script : public Asset {
  public:
    uint32_t initFromYaml(h_core::Assets* assets, YAML::Node node);

    uint32_t runMethodIfExists(asIScriptContext* context, std::string methodDecl);
    uint32_t runMethod(asIScriptContext* context, std::string methodDecl);

    std::string code {};
    std::string name {};

    asIScriptObject* instance = nullptr;
    asITypeInfo* type = nullptr;
};
}  // namespace script

}  // namespace h_core
