//
// Created by angus.goucher on 8/28/2024.
//
#pragma once

#include "angelscript.h"

#include "core/assetindex.h"
#include "core/assets.h"
#include "core/component.h"
#include "scriptasset.h"

namespace h_core {
namespace script {

class ScriptComp : public Component {
  public:
    ScriptComp() = default;

    uint32_t init(h_core::AssetIndex asset, h_core::Assets* assets, asIScriptContext* context, h_core::ActorId id);

    h_core::Assets* assets = nullptr;
    h_core::AssetIndex scriptAsset = ASSET_INDEX_BAD;
    asIScriptObject* instance = nullptr;

    uint32_t runMethodIfExists(
        asIScriptContext* context,
        const std::string& methodDecl) const;
    uint32_t runMethod(
        asIScriptContext* context,
        const std::string& methodDecl) const;

    HYCOMPONENT(2);
};

}
}