//
// Created by xavier.olmstead on 9/24/2024.
//
#include "core/yamloverrides.h"

std::vector<std::pair<std::string, int32_t>> h_core::YamlOverride::getActorOverrides(AssetIndex assetIndex) {
    return m_yamlOverrides[assetIndex];
}

void h_core::YamlOverride::parseActorOverrides(AssetIndex index, std::vector<YAML::Node> yaml) {
    std::vector<std::pair<std::string, int32_t>> overrideYamlValues {};

    for(YAML::Node overrideYaml : yaml) {
        std::string overridePath = overrideYaml["path"].as<std::string>("");
        int32_t overrideData = overrideYaml["value"].as<int32_t>(0);
        std::pair overridePair (overridePath, overrideData);

        overrideYamlValues.push_back(overridePair);
    }

    std::pair mapKeyValue (index, overrideYamlValues);
    m_yamlOverrides.insert(mapKeyValue);
}