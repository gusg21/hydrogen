//
// Created by angus.goucher on 8/28/2024.
//
#pragma once

#include "angelscript.h"

#include "core/assets.h"
#include "core/systems/script/scriptasset.h"

#define SCRIPT_COMPONENT_BITMASK (1 << 2)

namespace h_core {
namespace script {

class ScriptComp {
  public:
    uint32_t init(h_core::script::ScriptAsset* asset, asIScriptContext* context, h_core::ActorId id);

    h_core::script::ScriptAsset* scriptAsset;
    asIScriptObject* instance;

    uint32_t runMethodIfExists(
        asIScriptContext* context,
        const std::string& methodDecl) const;
    uint32_t runMethod(
        asIScriptContext* context,
        const std::string& methodDecl) const;
};

}
}