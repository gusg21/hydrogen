//
// Created by angus.goucher on 9/24/2024.
//

#pragma once

#include "core/actorspecasset.h"
#include "core/scenespecasset.h"
#include "editor/asseteditorwindow.h"
#include "editor/visuals/scenespecrenderer.h"

#include "yaml-cpp/yaml.h"

namespace h_editor {
namespace windows {

struct EditorActorSpec {
    EditorActorSpec() = default;
    EditorActorSpec(const h_core::ActorSpecAsset& spec, const h_core::AssetIndex index) : actorSpec(spec), index(index) {};

    h_core::ActorSpecAsset actorSpec {};
    h_core::AssetIndex index = ASSET_INDEX_BAD;
};

class SceneEditor : public h_editor::AssetEditorWindow {
public:
    explicit SceneEditor(h_editor::Editor* editor) : AssetEditorWindow(editor, "Scene Editor") {}

    void open(const std::string& assetPath) override;
    void close() override;
    void paintContent() override;
    void addActor(const YAML::Node& yaml, h_core::AssetIndex index);
    void saveToYaml();

    static SceneEditor* instance;

private:
    h_editor::visuals::SceneSpecRenderer* m_renderer = nullptr;
    YAML::Node m_sceneSpecYaml {};
    h_core::SceneSpecAsset m_spec {};
    std::vector<EditorActorSpec> m_actorSpecs {};
    std::string m_assetPath;
};

}
}
