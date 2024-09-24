//
// Created by angus.goucher on 9/24/2024.
//

#pragma once

#include "core/scenespecasset.h"
#include "editor/asseteditorwindow.h"

#include "yaml-cpp/yaml.h"

namespace h_editor {
namespace windows {

class SceneEditor : public h_editor::AssetEditorWindow {
public:
    explicit SceneEditor(h_editor::Editor* editor) : AssetEditorWindow(editor, "Scene Editor") {}

    void open(const std::string& assetPath) override;
    void paintContent() override;

private:
    YAML::Node m_sceneSpecYaml {};
    h_core::SceneSpecAsset spec {};
};

}
}
