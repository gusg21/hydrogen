#include "core/systems/script/scriptasset.h"

#include <fstream>
#include <sstream>

#include "core/actor.h"

uint32_t h_core::script::ScriptAsset::initFromYaml(
    h_core::Assets* assets, YAML::Node node) {
    std::string filePath = node["script_file"].as<std::string>("");

    std::string yamlName =
        node["script_name"].as<std::string>("UNNAMED_SCRIPT");

    if (filePath.empty()) {
        // Load script from text
        code = node["script"].as<std::string>("");
        name = yamlName;
    }
    else {
        // Load script from file
        std::ifstream scriptCodeFileStream { filePath };
        std::stringstream scriptCodeStream;
        scriptCodeStream << scriptCodeFileStream.rdbuf();
        code = scriptCodeStream.str();

        if (yamlName == "UNNAMED_SCRIPT") {
            size_t slashIndex = filePath.find_last_of('/');
            if (slashIndex != std::string::npos &&
                slashIndex < filePath.size() - 1) {
                name = filePath.substr(slashIndex + 1);
            }
            else {
                name = filePath;
            }
        }
        else {
            name = yamlName;
        }
    }

    printf("DEBUG: SCRIPT: chose name %s\n", name.c_str());
    printf("DEBUG: SCRIPT: \n%s\n", code.c_str());

    return 0;
}

// TODO: Chit-chat with Xav about whether these belong in script.cc or
// scripting.cc
uint32_t h_core::script::ScriptAsset::runMethodIfExists(
    asIScriptContext* context, asIScriptObject* instance,
    std::string methodDecl) {
    asIScriptFunction* func = type->GetMethodByDecl(methodDecl.c_str());

    if (func != nullptr) {
        // TODO(optimization): `func` is rederived in runMethod()
        return runMethod(context, instance, methodDecl);
    }

    return 0;
}

uint32_t h_core::script::ScriptAsset::runMethod(
    asIScriptContext* context, asIScriptObject* instance,
    std::string methodDecl) {
    asIScriptFunction* func = type->GetMethodByDecl(methodDecl.c_str());

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

// NOTE: Automatically AddRef()'s the instance created
asIScriptObject* h_core::script::ScriptAsset::constructInstance(
    asIScriptContext* context, h_core::ActorId id) const {
    // Construct object
    context->Prepare(typeConstructor);
    context->SetArgObject(0, &id);
    context->Execute();

    // Retrieve instance pointer
    asIScriptObject* instance =
        *(asIScriptObject**)context->GetAddressOfReturnValue();

    instance->AddRef();

    return instance;
}

uint32_t h_core::script::ScriptAsset::compile(asIScriptContext* context) {
    int result = module->Build();
    if (result != 0) {
        if (module->GetObjectTypeCount() > 0) {
            // Get the first type
            type = module->GetObjectTypeByIndex(0);
            // Determine constructor of form "MyClass @MyClass()"
            std::string typeName = type->GetName();
            std::string typeConstructorDecl =
                typeName + " @" + typeName + "(ActorId id)";
            typeConstructor =
                type->GetFactoryByDecl(typeConstructorDecl.c_str());
        }
        else {
            printf(
                "ERROR: SCRIPT: No defined types in module %s!\n",
                module->GetName());

            return SCRIPT_COMPILE_FAIL_NO_DEFINED_TYPES;
        }
    }
    else {
        printf("ERROR: SCRIPT: Failed to build module.\n");

        return SCRIPT_COMPILE_FAIL_BAD_BUILD;
    }

    return 0;
}
