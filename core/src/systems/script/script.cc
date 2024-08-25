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