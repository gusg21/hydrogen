//
// Created by angus.goucher on 8/28/2024.
//
#include "core/systems/script/scriptcomp.h"

uint32_t h_core::script::ScriptComp::init(
    h_core::script::ScriptAsset* asset, asIScriptContext* context, h_core::ActorId id) {
    scriptAsset = asset;
    instance = asset->constructInstance(context, id);

    return 0;
}
