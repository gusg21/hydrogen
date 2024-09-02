#include "core/project/project.h"

#include "core/input/inputactionsource.h"

void h_core::project::Project::loadFromFile(const std::string& yamlPath) {
    YAML::Node projectYaml = YAML::LoadFile(yamlPath);

    name = projectYaml["name"].as<std::string>("Unnamed Project");
    initialSceneSpec =
        projectYaml["initial_scene_spec"].as<h_core::AssetIndex>(ASSETS_ASSET_INDEX_BAD);

    for (YAML::Node assetInfo : projectYaml["assets"]) {
        requiredAssets.emplace_back(
            assetInfo["index"].as<uint32_t>(0),
            assetInfo["type"].as<uint32_t>(0),
            assetInfo["path"].as<std::string>(""));
    }

    for (YAML::Node actionInfo : projectYaml["input_actions"]) {
        ProjectActionEntry entry {};
        entry.name = actionInfo["name"].as<std::string>("");

        for (YAML::Node actionSourceInfo : actionInfo["sources"]) {
            h_core::project::ActionSourceEntry source {};
            if (actionSourceInfo["key"].IsDefined()) {
                source.type = h_core::input::InputActionSourceType::KEY;
                source.value.scanCode = (SDL_Scancode)actionSourceInfo["key"].as<uint32_t>(SDL_SCANCODE_0);
            }
            entry.sources.push_back(source);
        }

        actions.emplace_back(entry);
    }
}