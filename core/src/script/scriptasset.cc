#include "core/script/scriptasset.h"

#include "SDL.h"

#include "core/actorid.h"
#include "core/log.h"
#include "core/runtimesystems.h"
#include "core/script/scripting.h"

uint32_t h_core::script::ScriptAsset::initFromYaml(h_core::Assets* assets, const h_core::AssetDescription& desc, const YAML::Node& yaml) {
    h_core::Asset::initFromYaml(assets, desc, yaml);

    std::string filePath = yaml["file"].as<std::string>("");

    std::string yamlName = yaml["name"].as<std::string>("UNNAMED_SCRIPT");

    if (filePath.empty()) {
        // Load script from text
        code = yaml["code"].as<std::string>("");
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

    HYLOG_DEBUG("SCRIPT: chose name %s\n", name.c_str());
    HYLOG_DEBUG("SCRIPT: \n%s\n", code.c_str());

    return 0;
}

uint32_t h_core::script::ScriptAsset::precompile(h_core::RuntimeSystems* systems) {
    // Load + compile script
    loadCode(systems->scripting->getModule());

    // TODO: make each script comp have its own module so that we can hot-reload
    uint32_t result = compile(systems->scripting->getModule());
    if (result != 0) {
        return 1;
    }

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

void h_core::script::ScriptAsset::loadCode(asIScriptModule* module) const {
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
            HYLOG_ERROR("SCRIPT: No defined types in module %s!\n", module->GetName());

            return SCRIPT_COMPILE_FAIL_NO_DEFINED_TYPES;
        }
    }
    else {
        HYLOG_ERROR("SCRIPT: Failed to build module.\n");

        return SCRIPT_COMPILE_FAIL_BAD_BUILD;
    }

    return 0;
}
