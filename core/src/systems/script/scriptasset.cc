#include "core/systems/script/scriptasset.h"

#include "SDL.h"

#include "core/actor.h"
#include "core/runtimesystems.h"
#include "core/systems/script/scripting.h"

uint32_t h_core::script::ScriptAsset::initFromYaml(h_core::Assets* assets, YAML::Node node) {
    std::string filePath = node["file"].as<std::string>("");

    std::string yamlName = node["name"].as<std::string>("UNNAMED_SCRIPT");

    if (filePath.empty()) {
        // Load script from text
        code = node["code"].as<std::string>("");
        name = yamlName;
    }
    else {
        // Load script from file
        code = (const char*)SDL_LoadFile(filePath.c_str(), nullptr);

        if (yamlName == "UNNAMED_SCRIPT") {
            size_t slashIndex = filePath.find_last_of('/');
            if (slashIndex != std::string::npos && slashIndex < filePath.size() - 1) {
                name = filePath.substr(slashIndex + 1);
            }
            else { name = filePath; }
        }
        else { name = yamlName; }
    }

    ::SDL_Log("DEBUG: SCRIPT: chose name %s\n", name.c_str());
    ::SDL_Log("DEBUG: SCRIPT: \n%s\n", code.c_str());

    return 0;
}

uint32_t h_core::script::ScriptAsset::precompile(h_core::RuntimeSystems* systems) {
    // Load + compile script
    loadCode(systems->scripting->getModule());
    compile(systems->scripting->getModule());

    return 0;
}

// NOTE: Automatically AddRef()'s the instance created
asIScriptObject* h_core::script::ScriptAsset::constructInstance(asIScriptContext* context, h_core::ActorId id) const {
    // Construct object
    context->Prepare(typeConstructor);
    context->SetArgObject(0, &id);
    context->Execute();

    // Retrieve instance pointer
    asIScriptObject* instance = *(asIScriptObject**)context->GetAddressOfReturnValue();

    instance->AddRef();

    return instance;
}

void h_core::script::ScriptAsset::loadCode(asIScriptModule* module) {
    module->AddScriptSection(name.c_str(), code.c_str());
}

uint32_t h_core::script::ScriptAsset::compile(asIScriptModule* module) {
    int result = module->Build();

    if (result >= 0) {
        if (module->GetObjectTypeCount() > 0) {
            // Get the first type
            type = module->GetObjectTypeByIndex(0);
            // Determine constructor of form "MyClass@ MyClass()"
            std::string typeName = type->GetName();
            std::string typeConstructorDecl = typeName + "@ " + typeName + "(ActorId id)";
            typeConstructor = type->GetFactoryByDecl(typeConstructorDecl.c_str());
        }
        else {
            ::SDL_Log("ERROR: SCRIPT: No defined types in module %s!\n", module->GetName());

            return SCRIPT_COMPILE_FAIL_NO_DEFINED_TYPES;
        }
    }
    else {
        ::SDL_Log("ERROR: SCRIPT: Failed to build module.\n");

        return SCRIPT_COMPILE_FAIL_BAD_BUILD;
    }

    return 0;
}
