//
// Created by angus.goucher on 8/28/2024.
//
#include "core/systems/script/scriptcomp.h"

uint32_t h_core::script::ScriptComp::init(
    h_core::script::ScriptAsset* asset, asIScriptContext* context,
    h_core::ActorId id) {
    scriptAsset = asset;
    instance = asset->constructInstance(context, id);

    return 0;
}

uint32_t h_core::script::ScriptComp::runMethodIfExists(
    asIScriptContext* context, const std::string& methodDecl) const {
    asIScriptFunction* func = scriptAsset->type->GetMethodByDecl(methodDecl.c_str());

    if (func != nullptr) {
        // TODO(optimization): `func` is rederived in runMethod()
        return runMethod(context, methodDecl);
    }

    return 0;
}

uint32_t h_core::script::ScriptComp::runMethod(
    asIScriptContext* context, const std::string& methodDecl) const {
    asIScriptFunction* func = scriptAsset->type->GetMethodByDecl(methodDecl.c_str());

    int result;

    result = context->Prepare(func);
    if (result != 0) {
        // TODO: AngelScript returns more specific error codes, probably
        // worth it to specialize this error code if we ever use it
        return SCRIPT_RUN_METHOD_FAIL_BAD_PREPARE;
    }

    result = context->SetObject(instance);
    if (result != 0) {
        // TODO: See above
        return SCRIPT_RUN_METHOD_FAIL_BAD_INSTANCE;
    }

    result = context->Execute();
    if (result != 0) {
        // TODO: See above
        return SCRIPT_RUN_METHOD_FAIL_BAD_EXECUTE;
    }

    return 0;
}
