//
// Created by xavier.olmstead on 9/24/2024.
//

#pragma once
#include <yaml-cpp/yaml.h>
#include <unordered_map>

#include "core/assetindex.h"

namespace h_core {
class YamlOverride {
public:
    std::vector<std::pair<std::string, int32_t>> getActorOverrides(AssetIndex assetIndex);

    void parseActorOverrides(AssetIndex index, std::vector<YAML::Node> yaml);
private :
    std::unordered_map<std::string, int32_t> m_intOverrides {};
};
}
