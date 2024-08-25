#include "core/systems/script/script.h"

#include <fstream>
#include <sstream>

uint32_t h_core::script::Script::initFromYaml(
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
            else { name = filePath; }
        }
        else { name = yamlName; }
    }

    printf("DEBUG: SCRIPT: chose name %s\n", name.c_str());
    printf("DEBUG: SCRIPT: \n%s\n", code.c_str());

    return 0;
}

// TODO: Chit-chat with Xav about whether these belong in script.cc or
// scripting.cc
uint32_t h_core::script::Script::runMethodIfExists(
    asIScriptContext* context, std::string methodDecl) {
    asIScriptFunction* func = type->GetMethodByDecl(methodDecl.c_str());

    if (func != nullptr) {
        // TODO(optimization): `func` is rederived in runMethod()
        return runMethod(context, methodDecl);
    }

    return 0;
}

uint32_t h_core::script::Script::runMethod(
    asIScriptContext* context, std::string methodDecl) {
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