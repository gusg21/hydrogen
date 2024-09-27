#include "core/project/project.h"

#include "core/input/inputactionsource.h"
#include "core/log.h"

uint32_t h_core::project::Project::loadFromFile(const std::string& yamlPath, const std::string& assetsBasePath) {
    HYLOG_INFO("PROJECT: Loading from path %s\n", yamlPath.c_str());
    HYLOG_INFO("PROJECT: assetsBasePath = %s\n", assetsBasePath.c_str());

    // Determine full path
    std::string projectYamlPath = assetsBasePath + yamlPath;
    HYLOG_INFO("PROJECT: Full Project YAML Path = %s\n", projectYamlPath.c_str());

    // Load Text from file
    const char* projectYamlText = static_cast<const char*>(SDL_LoadFile(projectYamlPath.c_str(), nullptr));
    if (projectYamlText == nullptr) {
        HYLOG_INFO("PROJECT: Failed to load project from %s\n", projectYamlPath.c_str());
        return 1;  // TODO: Make into real error codes
    }
    HYLOG_INFO("PROJECT: YAML: %s\n", projectYamlText);

    // Parse YAML
    YAML::Node projectYaml;
    // I hate exceptions!
    try {
        projectYaml = YAML::Load(projectYamlText);
    } catch (YAML::ParserException&) {
        HYLOG_INFO("PROJECT: Failed to parse YAML from %s\n", projectYamlPath.c_str());
        return 1;  // TODO: Make into real error codes
    }

    // Load data
    name = projectYaml["name"].as<std::string>("Unnamed Project");
    initialSceneSpec = projectYaml["initial_scene_spec"].as<h_core::AssetIndex>(ASSET_INDEX_BAD);
    assetServerAddress = projectYaml["asset_server_address"].as<std::string>("");

    // Load assets
    for (YAML::Node assetInfo : projectYaml["assets"]) {
        requiredAssets.emplace_back(
            assetInfo["index"].as<uint32_t>(0), assetInfo["type"].as<uint32_t>(0),
            assetInfo["path"].as<std::string>(""), static_cast<AssetRemoteMode>(assetInfo["remote"].as<uint8_t>(0)));
    }

    // Load input actions
    for (YAML::Node actionInfo : projectYaml["input_actions"]) {
        ProjectActionEntry entry {};
        entry.name = actionInfo["name"].as<std::string>("");

        for (YAML::Node actionSourceInfo : actionInfo["sources"]) {
            h_core::project::ProjectActionSourceEntry source {};
            if (actionSourceInfo["key"].IsDefined()) {
                source.type = h_core::input::InputActionSourceType::KEY;
                source.value.scanCode = static_cast<SDL_Scancode>(actionSourceInfo["key"].as<uint32_t>(SDL_SCANCODE_0));
            }
            else if (actionSourceInfo["negative_key"].IsDefined() && actionSourceInfo["positive_key"].IsDefined()) {
                source.type = h_core::input::InputActionSourceType::DUAL_KEY;
                source.value.dualScanCodes[0] =
                    static_cast<SDL_Scancode>(actionSourceInfo["negative_key"].as<uint32_t>(SDL_SCANCODE_0));
                source.value.dualScanCodes[1] =
                    static_cast<SDL_Scancode>(actionSourceInfo["positive_key"].as<uint32_t>(SDL_SCANCODE_0));
            }
            entry.sources.push_back(source);
        }

        actions.emplace_back(entry);
    }

    baseProjectPath = assetsBasePath;


    return 0;
}

bool h_core::project::Project::hasAssetPath(const std::string& assetPath) const {
    for (const AssetDescription& entry : requiredAssets) {
        if (entry.path == assetPath) { return true; }
    }
    return false;
}

std::string h_core::project::Project::getPathByIndex(AssetIndex index) const {
    for (const AssetDescription& entry : requiredAssets) {
        if (entry.index == index) { return entry.path; }
    }

    return "";
}

h_core::AssetIndex h_core::project::Project::getOpenIndex() const {
    for (AssetIndex index = 0; index < UINT32_MAX; index++) {
        bool indexOccupied = false;
        for (const AssetDescription& entry : requiredAssets) {
            if (entry.index == index) { indexOccupied = true; }
        }
        if (!indexOccupied) { return index; }
    }
    return ASSET_INDEX_BAD;
}

void h_core::project::Project::removeByPath(const std::string& assetPath) {
    auto found = requiredAssets.end();
    for (auto entryIter = requiredAssets.begin(); entryIter < requiredAssets.end(); ++entryIter) {
        if (entryIter->path == assetPath) {
            found = entryIter;
            break;
        }
    }
    if (found != requiredAssets.end()) { requiredAssets.erase(found); }
}
void h_core::project::Project::saveToYaml(YAML::Node& yaml) {
    yaml["initial_scene_spec"] = initialSceneSpec;
    yaml["name"] = name;
    yaml["asset_server_address"] = assetServerAddress;
    std::vector<YAML::Node> yamlInputActions {};

    // Actions
    for (auto action : actions) {
        YAML::Node yamlAction {};
        yamlAction["name"] = action.name;

        std::vector<YAML::Node> yamlSources {};
        for (auto source : action.sources) {
            YAML::Node yamlSource {};
            switch (source.type) {
                case input::InputActionSourceType::KEY:
                    yamlSource["key"] = (uint32_t)source.value.scanCode;
                    break;
                case input::InputActionSourceType::DUAL_KEY:
                    yamlSource["negative_key"] = (uint32_t)source.value.dualScanCodes[0];
                    yamlSource["positive_key"] = (uint32_t)source.value.dualScanCodes[1];
                    break;
            }
            yamlSources.push_back(yamlSource);
        }
        yamlAction["sources"] = yamlSources;
        yamlInputActions.push_back(yamlAction);
    }
    yaml["input_actions"] = yamlInputActions;

    // Assets
    std::vector<YAML::Node> yamlAssets {};
    for (auto assetData : requiredAssets) {
        YAML::Node yamlAsset {};
        yamlAsset["index"] = assetData.index;
        yamlAsset["type"] = assetData.type;

        if (assetData.remote == AssetRemoteMode::LOCAL) { yamlAsset["path"] = assetData.path; }
        else { yamlAsset["remote"] = (uint32_t)assetData.remote; }

        yamlAssets.push_back(yamlAsset);
    }
    yaml["assets"] = yamlAssets;
}
