#include "core/systems/script/scripting.h"

#include "scriptstdstring.h"

#include "core/math/math.h"
#include "core/transform.h"
#include "core/systems/script/scriptcomp.h"

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

std::string actorIdToString(h_core::ActorId id) {
    return std::to_string(id);
}

uint32_t h_core::script::Scripting::init(h_core::Engine* engine) {
    // Setup engine
    scriptEngine = asCreateScriptEngine(SCRIPTING_ANGELSCRIPT_VERSION);
    scriptModule =
        scriptEngine->GetModule("hyScripting", asGM_CREATE_IF_NOT_EXISTS);
    scriptContext = scriptEngine->CreateContext();

    // Engine utils
    RegisterStdString(scriptEngine);
    scriptEngine->SetMessageCallback(
        asFUNCTION(angelScriptMessageCallback), nullptr, asCALL_CDECL);
    scriptEngine->RegisterGlobalFunction(
        "void print(const string &in)", asFUNCTION(angelScriptPrint),
        asCALL_CDECL);


    // ENGINE INTERFACE BEGIN

    scriptEngine->SetDefaultNamespace("engine");

    // -> Types
    scriptEngine->RegisterObjectType(
        "Vector3", sizeof(h_core::math::Vector3),
        asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<h_core::math::Vector3>());
    scriptEngine->RegisterObjectProperty(
        "Vector3", "float x", asOFFSET(h_core::math::Vector3, x));
    scriptEngine->RegisterObjectProperty(
        "Vector3", "float y", asOFFSET(h_core::math::Vector3, y));
    scriptEngine->RegisterObjectProperty(
        "Vector3", "float z", asOFFSET(h_core::math::Vector3, z));

    scriptEngine->RegisterObjectType(
        "Transform", sizeof(h_core::Transform),
        asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<h_core::Transform>());
    scriptEngine->RegisterObjectProperty(
        "Transform", "Vector3 position", asOFFSET(h_core::Transform, position));
    scriptEngine->RegisterObjectProperty(
        "Transform", "Vector3 scale", asOFFSET(h_core::Transform, scale));

    scriptEngine->RegisterObjectType(
        "ActorId", sizeof(h_core::ActorId),
        asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<h_core::ActorId>());

    // -> Global Functions
    scriptEngine->RegisterGlobalFunction(
        "Transform getBoundTransform()", asMETHOD(Scripting, getBoundTransform),
        asCALL_THISCALL_ASGLOBAL, this);
    scriptEngine->RegisterGlobalFunction(
        "void setBoundTransform(Transform)",
        asMETHOD(Scripting, setBoundTransform), asCALL_THISCALL_ASGLOBAL, this);
    scriptEngine->RegisterGlobalFunction(
        "string toString(ActorId id)",
        asFUNCTIONPR(actorIdToString, (ActorId), std::string), asCALL_CDECL);

    // ENGINE INTERFACE END

    return 0;
}

void h_core::script::Scripting::destroy() {}

void h_core::script::Scripting::initPerActor() {

}

void h_core::script::Scripting::beginFrame() {}

void h_core::script::Scripting::process() {
    // Build script if the script is not yet built
    // TODO: Is this good? Might be better to have a post-init hook for systems

    script->runMethodIfExists(scriptContext, "void process()");
}

void h_core::script::Scripting::draw() {
    script->runMethodIfExists(scriptContext, "void draw()");
}

void h_core::script::Scripting::endFrame() {}

uint32_t h_core::script::Scripting::getMask() {
    return SCRIPT_COMPONENT_BITMASK;
}

asIScriptContext* h_core::script::Scripting::getContext() {
    return scriptContext;
}

h_core::Transform h_core::script::Scripting::getBoundTransform() {
    return *transform;
}

void h_core::script::Scripting::setBoundTransform(h_core::Transform newTrans) {
    *transform = newTrans;
}