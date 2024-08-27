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

h_core::math::Vector3 newVector3(float x, float y, float z) {
    return h_core::math::Vector3 { x, y, z };
}

void angelScriptPrint(std::string& message) {
    printf("INFO: SCRIPTING: %s\n", message.c_str());
}

std::string actorIdToString(h_core::ActorId id) {
    return std::to_string(id);
}

void RegisterVector3(asIScriptEngine* scriptingEngine) {
    scriptingEngine->RegisterObjectType(
        "Vector3", sizeof(h_core::math::Vector3),
        asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<h_core::math::Vector3>());
    scriptingEngine->RegisterObjectProperty(
        "Vector3", "float x", asOFFSET(h_core::math::Vector3, x));
    scriptingEngine->RegisterObjectProperty(
        "Vector3", "float y", asOFFSET(h_core::math::Vector3, y));
    scriptingEngine->RegisterObjectProperty(
        "Vector3", "float z", asOFFSET(h_core::math::Vector3, z));

    scriptingEngine->RegisterGlobalFunction(
        "float dot(Vector3 a, Vector3 b)",
        asFUNCTION(h_core::math::Vector3::dot), asCALL_CDECL);

    // vector constructor
    scriptingEngine->RegisterGlobalFunction(
        "Vector3 newVec3(float x, float y, float z)", asFUNCTION(newVector3),
        asCALL_CDECL);
}

void RegisterQuaternion(asIScriptEngine* scriptingEngine) {
    scriptingEngine->RegisterObjectType(
        "Quaternion", sizeof(h_core::math::Quaternion),
        asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<h_core::math::Quaternion>());

    scriptingEngine->RegisterObjectProperty(
        "Quaternion", "float x", asOFFSET(h_core::math::Quaternion, x));
    scriptingEngine->RegisterObjectProperty(
        "Quaternion", "float y", asOFFSET(h_core::math::Quaternion, y));
    scriptingEngine->RegisterObjectProperty(
        "Quaternion", "float z", asOFFSET(h_core::math::Quaternion, z));
    scriptingEngine->RegisterObjectProperty(
        "Quaternion", "float w", asOFFSET(h_core::math::Quaternion, w));

    scriptingEngine->RegisterGlobalFunction(
        "Quaternion normalize(Quaternion quat)",
        asFUNCTION(h_core::math::Quaternion::normalize), asCALL_CDECL);
    scriptingEngine->RegisterGlobalFunction(
        "float dot(Quaternion quat)", asFUNCTION(h_core::math::Quaternion::dot),
        asCALL_CDECL);
    // TODO: not working for some reason...
    // scriptingEngine->RegisterGlobalFunction(
    //     "float getLength(Quaternion quat)",
    //     asFUNCTION(h_core::math::Quaternion::getLength), asCALL_CDECL);
    scriptingEngine->RegisterGlobalFunction(
        "Quaternion fromEuler(Vector3 euler)",
        asFUNCTION(h_core::math::Quaternion::fromEuler), asCALL_CDECL);
    scriptingEngine->RegisterGlobalFunction(
        "Vector3 toEuler(Quaternion quat)",
        asFUNCTION(h_core::math::Quaternion::toEuler), asCALL_CDECL);
    scriptingEngine->RegisterGlobalFunction(
        "Quaternion rotate(Quaternion quat, float angle, Vector3 axis)",
        asFUNCTION(h_core::math::Quaternion::rotate), asCALL_CDECL);
    scriptingEngine->RegisterGlobalFunction(
        "Quaternion rotate(Quaternion a, Quaternion b)",
        asFUNCTION(h_core::math::Quaternion::multiply), asCALL_CDECL);
}

uint32_t h_core::script::Scripting::init() {
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
        "Vector2", sizeof(h_core::math::Vector2),
        asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<h_core::math::Vector2>());
    scriptEngine->RegisterObjectProperty(
        "Vector2", "float x", asOFFSET(h_core::math::Vector2, x));
    scriptEngine->RegisterObjectProperty(
        "Vector2", "float y", asOFFSET(h_core::math::Vector2, y));


    RegisterQuaternion(scriptEngine);

    scriptEngine->RegisterObjectType(
        "Transform", sizeof(h_core::Transform),
        asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<h_core::Transform>());
    scriptEngine->RegisterObjectProperty(
        "Transform", "Vector3 position", asOFFSET(h_core::Transform, position));
    scriptEngine->RegisterObjectProperty(
        "Transform", "Quaternion rotation",
        asOFFSET(h_core::Transform, rotation));
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
    scriptModule->AddScriptSection(script->name.c_str(), script->code.c_str());
    scriptModuleBuilt = false;
}

void h_core::script::Scripting::beginFrame() {}

void h_core::script::Scripting::process() {
    // Build script if the script is not yet built
    // TODO: Is this good? Might be better to have a post-init hook for systems
    if (!scriptModuleBuilt) {
        int result = scriptModule->Build();
        if (!(result < 0)) {
            if (scriptModule->GetObjectTypeCount() > 0) {
                // Get the first type
                script->type = scriptModule->GetObjectTypeByIndex(0);
                // Determine constructor of form "MyClass @MyClass()"
                std::string typeName = script->type->GetName();
                std::string typeConstructorDecl =
                    typeName + " @" + typeName + "(ActorId id)";
                asIScriptFunction* typeConstructor =
                    script->type->GetFactoryByDecl(typeConstructorDecl.c_str());

                // Construct object
                scriptContext->Prepare(typeConstructor);
                scriptContext->SetArgObject(0, &actorId);
                scriptContext->Execute();

                // Retreive instance pointer
                script->instance =
                    *(asIScriptObject**)
                         scriptContext->GetAddressOfReturnValue();

                script->instance->AddRef();  // We're holding onto it

                scriptModuleBuilt = true;
            }
            else {
                printf(
                    "ERROR: SCRIPTING: No defined types in module %s!\n",
                    scriptModule->GetName());
            }
        }
        else { printf("ERROR: SCRIPTING: Failed to build module.\n"); }
    }

    script->runMethodIfExists(scriptContext, "void process()");
}

void h_core::script::Scripting::draw() {
    script->runMethodIfExists(scriptContext, "void draw()");
}

void h_core::script::Scripting::endFrame() {}

uint32_t h_core::script::Scripting::getMask() {
    return SCRIPT_COMPONENT_BITMASK;
}

h_core::Transform h_core::script::Scripting::getBoundTransform() {
    return *transform;
}

void h_core::script::Scripting::setBoundTransform(h_core::Transform newTrans) {
    *transform = newTrans;
}