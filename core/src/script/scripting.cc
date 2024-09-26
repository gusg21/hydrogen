#include "core/script/scripting.h"

#include "scriptstdstring.h"

#include "core/engine.h"
#include "core/log.h"
#include "core/math/math.h"
#include "core/render/renderer.h"
#include "core/runtimeengine.h"
#include "core/script/scriptcomp.h"
#include "core/transform.h"

#define SCRIPTING_ANGELSCRIPT_VERSION 23700  // AngelScript v23.7.0

void AssetDescriptionConstructor(void* memory) {
    new (memory) h_core::AssetDescription();
}

void angelScriptMessageCallback(const asSMessageInfo* message, void* params) {
    switch (message->type) {
        case asMSGTYPE_ERROR:
            HYLOG_ERROR("SCRIPTING: %s:%d,%d\n%s\n", message->section, message->row, message->col, message->message);
            break;
        case asMSGTYPE_WARNING:
            HYLOG_WARN("SCRIPTING: %s:%d,%d\n%s\n", message->section, message->row, message->col, message->message);
            break;
        case asMSGTYPE_INFORMATION:
            HYLOG_INFO("SCRIPTING: %s:%d,%d\n%s\n", message->section, message->row, message->col, message->message);
            break;

        default:
            HYLOG_ERROR("SCRIPTING: %s:%d,%d\n%s\n", message->section, message->row, message->col, message->message);
            break;
    }
}

h_core::math::Vector3 newVector3(float x, float y, float z) {
    return h_core::math::Vector3 { x, y, z };
}

h_core::AssetDescription newAssetDescription(h_core::AssetIndex index, int type, h_core::AssetRemoteMode remote) {
    return h_core::AssetDescription(index, type, "", remote);
}

void angelScriptPrint(std::string& message) {
    HYLOG_INFO("SCRIPTING: %s\n", message.c_str());
}

std::string actorIdToString(h_core::ActorId id) {
    return std::to_string(id);
}

void loadAsset(h_core::AssetDescription& desc) {}

void RegisterVector3(asIScriptEngine* scriptingEngine) {
    scriptingEngine->RegisterObjectType(
        "Vector3", sizeof(h_core::math::Vector3), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<h_core::math::Vector3>());
    scriptingEngine->RegisterObjectProperty("Vector3", "float x", asOFFSET(h_core::math::Vector3, x));
    scriptingEngine->RegisterObjectProperty("Vector3", "float y", asOFFSET(h_core::math::Vector3, y));
    scriptingEngine->RegisterObjectProperty("Vector3", "float z", asOFFSET(h_core::math::Vector3, z));

    scriptingEngine->RegisterGlobalFunction(
        "float dot(Vector3 a, Vector3 b)",
        asFUNCTIONPR(h_core::math::Vector3::dot, (h_core::math::Vector3, h_core::math::Vector3), float), asCALL_CDECL);

    // vector constructor
    scriptingEngine->RegisterGlobalFunction(
        "Vector3 newVector3(float x, float y, float z)", asFUNCTION(newVector3), asCALL_CDECL);
}

void RegisterQuaternion(asIScriptEngine* scriptingEngine) {
    scriptingEngine->RegisterObjectType(
        "Quaternion", sizeof(h_core::math::Quaternion),
        asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<h_core::math::Quaternion>());

    scriptingEngine->RegisterObjectProperty("Quaternion", "float x", asOFFSET(h_core::math::Quaternion, x));
    scriptingEngine->RegisterObjectProperty("Quaternion", "float y", asOFFSET(h_core::math::Quaternion, y));
    scriptingEngine->RegisterObjectProperty("Quaternion", "float z", asOFFSET(h_core::math::Quaternion, z));
    scriptingEngine->RegisterObjectProperty("Quaternion", "float w", asOFFSET(h_core::math::Quaternion, w));

    scriptingEngine->RegisterGlobalFunction(
        "Quaternion normalize(Quaternion quat)", asFUNCTION(h_core::math::Quaternion::normalize), asCALL_CDECL);
    scriptingEngine->RegisterGlobalFunction(
        "float dot(Quaternion quat)", asFUNCTION(h_core::math::Quaternion::dot), asCALL_CDECL);
    // TODO: not working for some reason...
    // scriptingEngine->RegisterGlobalFunction(
    //     "float getLength(Quaternion quat)",
    //     asFUNCTION(h_core::math::Quaternion::getLength), asCALL_CDECL);
    scriptingEngine->RegisterGlobalFunction(
        "Quaternion fromEuler(Vector3 euler)", asFUNCTION(h_core::math::Quaternion::fromEuler), asCALL_CDECL);
    scriptingEngine->RegisterGlobalFunction(
        "Vector3 toEuler(Quaternion quat)", asFUNCTION(h_core::math::Quaternion::toEuler), asCALL_CDECL);
    scriptingEngine->RegisterGlobalFunction(
        "Quaternion rotate(Quaternion quat, float angle, Vector3 axis)", asFUNCTION(h_core::math::Quaternion::rotate),
        asCALL_CDECL);
    scriptingEngine->RegisterGlobalFunction(
        "Quaternion rotate(Quaternion a, Quaternion b)", asFUNCTION(h_core::math::Quaternion::multiply), asCALL_CDECL);
}

void RegisterAssetDescription(asIScriptEngine* scriptingEngine) {
    // Class
    SCRIPTING_ASVERIFY(scriptingEngine->RegisterObjectType(
        "AssetDescription", sizeof(h_core::AssetDescription),
        asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<h_core::AssetDescription>()));

    // Class data
    SCRIPTING_ASVERIFY(scriptingEngine->RegisterObjectProperty(
        "AssetDescription", "AssetIndex index", asOFFSET(h_core::AssetDescription, index)));
    SCRIPTING_ASVERIFY(scriptingEngine->RegisterObjectProperty(
        "AssetDescription", "int type", asOFFSET(h_core::AssetDescription, type)));
    SCRIPTING_ASVERIFY(scriptingEngine->RegisterObjectProperty(
        "AssetDescription", "AssetRemoteMode remote", asOFFSET(h_core::AssetDescription, remote)));
    SCRIPTING_ASVERIFY(scriptingEngine->RegisterObjectProperty(
        "AssetDescription", "string path", asOFFSET(h_core::AssetDescription, path)));

    // Constructor
    SCRIPTING_ASVERIFY(scriptingEngine->RegisterGlobalFunction(
        "AssetDescription newAssetDescription(AssetIndex index, int type, AssetRemoteMode remote)",
        asFUNCTION(newAssetDescription), asCALL_CDECL));
}

void RegisterAssetRemoteModeEnum(asIScriptEngine* scriptingEngine) {
    scriptingEngine->RegisterEnum("AssetRemoteMode");
    scriptingEngine->RegisterEnumValue("AssetRemoteMode", "LOCAL", 0);
    scriptingEngine->RegisterEnumValue("AssetRemoteMode", "REMOTE_IMMEDIATE", 1);
    scriptingEngine->RegisterEnumValue("AssetRemoteMode", "REMOTE_ON_REQUEST", 2);
}

uint32_t h_core::script::Scripting::init(h_core::RuntimeEngine* engine) {
    h_core::RuntimeSystem::init(engine);

    // Setup engine
    m_scriptEngine = ::asCreateScriptEngine(SCRIPTING_ANGELSCRIPT_VERSION);
    m_scriptModule = m_scriptEngine->GetModule("hyScripting", asGM_CREATE_IF_NOT_EXISTS);
    m_scriptContext = m_scriptEngine->CreateContext();

    // Engine utils
    ::RegisterStdString(m_scriptEngine);
    m_scriptEngine->SetMessageCallback(asFUNCTION(angelScriptMessageCallback), nullptr, asCALL_CDECL);
    m_scriptEngine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(angelScriptPrint), asCALL_CDECL);

    // ENGINE INTERFACE BEGIN
    m_scriptEngine->SetDefaultNamespace("engine");

    // -> Types

    // Vector2
    m_scriptEngine->RegisterObjectType(
        "Vector2", sizeof(h_core::math::Vector2), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<h_core::math::Vector2>());
    m_scriptEngine->RegisterObjectProperty("Vector2", "float x", asOFFSET(h_core::math::Vector2, x));
    m_scriptEngine->RegisterObjectProperty("Vector2", "float y", asOFFSET(h_core::math::Vector2, y));
    m_scriptEngine->RegisterTypedef("AssetIndex", "int");

    RegisterAssetRemoteModeEnum(m_scriptEngine);
    RegisterVector3(m_scriptEngine);
    RegisterQuaternion(m_scriptEngine);
    RegisterAssetDescription(m_scriptEngine);

    // Transform
    m_scriptEngine->RegisterObjectType(
        "Transform", sizeof(h_core::Transform), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<h_core::Transform>());
    m_scriptEngine->RegisterObjectProperty("Transform", "Vector3 position", asOFFSET(h_core::Transform, position));
    m_scriptEngine->RegisterObjectProperty("Transform", "Quaternion rotation", asOFFSET(h_core::Transform, rotation));
    m_scriptEngine->RegisterObjectProperty("Transform", "Vector3 scale", asOFFSET(h_core::Transform, scale));

    // ActorId
    m_scriptEngine->RegisterObjectType(
        "ActorId", sizeof(h_core::ActorId), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_PRIMITIVE);

    // -> Global Functions
    m_scriptEngine->RegisterGlobalFunction(
        "Transform getBoundTransform()", asMETHOD(Scripting, getBoundTransform), asCALL_THISCALL_ASGLOBAL, this);
    m_scriptEngine->RegisterGlobalFunction(
        "void setBoundTransform(Transform trans)", asMETHOD(Scripting, setBoundTransform), asCALL_THISCALL_ASGLOBAL,
        this);
    m_scriptEngine->RegisterGlobalFunction(
        "string toString(ActorId id)", asFUNCTIONPR(actorIdToString, (ActorId), std::string), asCALL_CDECL);
    m_scriptEngine->RegisterGlobalFunction(
        "double getDeltaTime()", asMETHOD(Engine, getDeltaSecs), asCALL_THISCALL_ASGLOBAL, engine);
    m_scriptEngine->RegisterGlobalFunction(
        "float getDistanceToCamera()", asMETHOD(Scripting, getDistanceToCamera), asCALL_THISCALL_ASGLOBAL, this);
    m_scriptEngine->RegisterGlobalFunction(
        "AssetIndex getBoundModel()", asMETHOD(Scripting, getBoundModel), asCALL_THISCALL_ASGLOBAL, this);
    m_scriptEngine->RegisterGlobalFunction(
        "void loadAsset(AssetDescription desc)", asMETHOD(Scripting, loadAsset), asCALL_THISCALL_ASGLOBAL, this);

    // ENGINE INTERFACE END

    return 0;
}

void h_core::script::Scripting::destroy() {}

void h_core::script::Scripting::initPerActor() {}

void h_core::script::Scripting::beginFrame() {}

void h_core::script::Scripting::process() {
    // Build script if the script is not yet built
    // TODO: Is this good? Might be better to have a post-init hook for systems

    if (script->scriptAsset == ASSET_INDEX_BAD) return;
    script->runMethodIfExists(m_scriptContext, "void process()");
}

void h_core::script::Scripting::draw() {
    if (script->scriptAsset == ASSET_INDEX_BAD) return;
    script->runMethodIfExists(m_scriptContext, "void draw()");
}

void h_core::script::Scripting::endFrame() {}

asIScriptContext* h_core::script::Scripting::getContext() const {
    return m_scriptContext;
}

h_core::Transform h_core::script::Scripting::getBoundTransform() {
    return *transform;
}

h_core::AssetIndex h_core::script::Scripting::getBoundModel() {
    return meshComp->model;
}

void h_core::script::Scripting::setBoundTransform(h_core::Transform newTrans) {
    *transform = newTrans;
}

asIScriptModule* h_core::script::Scripting::getModule() const {
    return m_scriptModule;
}

float h_core::script::Scripting::getDistanceToCamera() {
    return (transform->position - engine->getSystems()->renderer->getCameraPosition()).getLength();
}

void h_core::script::Scripting::loadAsset(AssetDescription desc) {
    engine->getRuntimeAssets()->loadAsset(desc);
}