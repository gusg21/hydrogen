#include "core/project.h"

void h_core::Project::initFromYaml(std::string yamlPath) {
    YAML::Node projectYaml = YAML::LoadFile(yamlPath);

    name = projectYaml["name"].as<std::string>("Unnamed Project");
    initialSceneSpec = projectYaml["initialSceneSpec"].as<h_core::AssetIndex>(0);

    for (YAML::Node assetInfo : projectYaml["assets"]) {
        assets.push_back(h_core::ProjectAssetEntry {
            assetInfo["index"].as<uint32_t>(0),
            assetInfo["typeId"].as<uint32_t>(0),
            assetInfo["path"].as<std::string>("")
        });
    }
}