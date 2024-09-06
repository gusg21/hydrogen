#include "core/project/project.h"

#include "core/input/inputactionsource.h"

void h_core::project::Project::loadFromFile(const std::string& yamlPath, const std::string& assetsBasePath) {
    ::SDL_Log("INFO: PROJECT: Loading from path %s\n", yamlPath.c_str());
    ::SDL_Log("INFO: PROJECT: !SDL_GetBasePath() = %s\n", ::SDL_GetBasePath());
    std::string projectYamlPath = assetsBasePath + yamlPath;
    ::SDL_Log("INFO: PROJECT: Full Project YAML Path = %s\n", projectYamlPath.c_str());
    const char* projectYamlText = (const char*)SDL_LoadFile(projectYamlPath.c_str(), nullptr);
    ::SDL_Log("INFO: PROJECT: YAML: %s\n", projectYamlText);
    YAML::Node projectYaml = YAML::Load(projectYamlText);

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
            h_core::project::ProjectActionSourceEntry source {};
            if (actionSourceInfo["key"].IsDefined()) {
                source.type = h_core::input::InputActionSourceType::KEY;
                source.value.scanCode = (SDL_Scancode)actionSourceInfo["key"].as<uint32_t>(SDL_SCANCODE_0);
            }
            else if (actionSourceInfo["negative_key"].IsDefined() && actionSourceInfo["positive_key"].IsDefined()) {
                source.type = h_core::input::InputActionSourceType::DUAL_KEY;
                source.value.dualScanCodes[0] = (SDL_Scancode)actionSourceInfo["negative_key"].as<uint32_t>(SDL_SCANCODE_0);
                source.value.dualScanCodes[1] = (SDL_Scancode)actionSourceInfo["positive_key"].as<uint32_t>(SDL_SCANCODE_0);
            }
            entry.sources.push_back(source);
        }

        actions.emplace_back(entry);
    }
}