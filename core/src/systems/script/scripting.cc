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
        "Transform", sizeof(h_core::Transform), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<h_core::Transform>());

    // -> Global Functions
    scriptEngine->RegisterGlobalFunction(
        "Transform getTransform()", asMETHOD(Scripting, getBoundTransform),
        asCALL_THISCALL_ASGLOBAL, this);

    // ENGINE INTERFACE END

    return 0;
}

void h_core::script::Scripting::destroy() {}

void h_core::script::Scripting::initPerActor() {
    scriptModule->AddScriptSection(script->name.c_str(), script->code.c_str());
    scriptModuleBuilt = false;
}

void h_core::script::Scripting::beginFrame() {
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
                    typeName + " @" + typeName + "()";
                asIScriptFunction* typeConstructor =
                    script->type->GetFactoryByDecl(typeConstructorDecl.c_str());

                // Construct object
                scriptContext->Prepare(typeConstructor);
                scriptContext->Execute();

                // Retreive instance pointer
                script->instance =
                    *(asIScriptObject**)
                         scriptContext->GetAddressOfReturnValue();

                script->instance->AddRef(); // We're holding onto it

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
}

void h_core::script::Scripting::process() {
    script->runMethodIfExists(scriptContext, "void process()");
}

void h_core::script::Scripting::draw() {}

void h_core::script::Scripting::endFrame() {}

uint32_t h_core::script::Scripting::getMask() {
    return SCRIPT_COMPONENT_BITMASK;
}

h_core::Transform h_core::script::Scripting::getBoundTransform() {
    return *transform;
}