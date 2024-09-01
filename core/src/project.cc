#include "core/project.h"

void h_core::Project::loadFromFile(const std::string& yamlPath) {
    YAML::Node projectYaml = YAML::LoadFile(yamlPath);

    name = projectYaml["name"].as<std::string>("Unnamed Project");
    initialSceneSpec = projectYaml["initialSceneSpec"].as<h_core::AssetIndex>(0);

    for (YAML::Node assetInfo : projectYaml["assets"]) {
        requiredAssets.emplace_back(
            assetInfo["index"].as<uint32_t>(0),
            assetInfo["type"].as<uint32_t>(0),
            assetInfo["path"].as<std::string>("")
        );
    }
}