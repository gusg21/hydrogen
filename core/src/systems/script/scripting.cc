#include "core/systems/script/scripting.h"

#include "scriptstdstring.h"

#include "core/systems/script/script.h"

#define SCRIPTING_ANGELSCRIPT_VERSION 23700  // AngelScript v23.7.0

void angelScriptMessageCallback(const asSMessageInfo* message, void* params) {
    const char* messageType;
    switch (message->type) {
        case asMSGTYPE_ERROR:
            messageType = "ERROR";
            break;
        case asMSGTYPE_WARNING:
            messageType = "WARN";
            break;
        case asMSGTYPE_INFORMATION:
            messageType = "INFO";
            break;

        default:
            messageType = "ERROR";
            break;
    }

    printf(
        "%s: SCRIPTING: %s:%d,%d\n%s\n", messageType, message->section,
        message->row, message->col, message->message);
}

void angelScriptPrint(std::string& message) {
    printf("INFO: SCRIPTING: %s\n", message.c_str());
}

uint32_t h_core::script::Scripting::init() {
    // Setup engine
    scriptEngine = asCreateScriptEngine(SCRIPTING_ANGELSCRIPT_VERSION);
    scriptModule =
        scriptEngine->GetModule("hyScripting", asGM_CREATE_IF_NOT_EXISTS);

    RegisterStdString(scriptEngine);

    // Register functions
    scriptEngine->SetMessageCallback(
        asFUNCTION(angelScriptMessageCallback), nullptr, asCALL_CDECL);
    scriptEngine->RegisterGlobalFunction(
        "void print(const string &in)", asFUNCTION(angelScriptPrint),
        asCALL_CDECL);

    return 0;
}

void h_core::script::Scripting::destroy() {}

void h_core::script::Scripting::initPerActor() {
    printf("FUCK %d\n", actorId);
    scriptModule->AddScriptSection(script->name.c_str(), script->code.c_str());
    scriptModuleBuilt = false;
}

void h_core::script::Scripting::beginFrame() {
    if (!scriptModuleBuilt) {
        int result = scriptModule->Build();
        if (result < 0) {
            printf("ERROR: SCRIPTING: Failed to build module.\n");
        }
        else { scriptModuleBuilt = true; }
    }
}

void h_core::script::Scripting::process() {
    asIScriptFunction* func =
        scriptModule->GetFunctionByDecl("void Test::doThaThang()");

    asIScriptContext* context = scriptEngine->CreateContext();
    context->Prepare(func);
    int result = context->Execute();
}

void h_core::script::Scripting::draw() {}

void h_core::script::Scripting::endFrame() {}

uint32_t h_core::script::Scripting::getMask() {
    return SCRIPT_COMPONENT_BITMASK;
}
