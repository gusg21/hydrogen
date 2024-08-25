#pragma once

#include "yaml-cpp/yaml.h"

#include <string>

#include "core/asset.h"

#define SCRIPT_COMPONENT_BITMASK (1 << 2)

namespace h_core {
namespace script {

class Script : public Asset {
  public:
    uint32_t initFromYaml(h_core::Assets* assets, YAML::Node node);

    std::string code {};
    std::string name {};
};
}  // namespace script

}  // namespace h_core
