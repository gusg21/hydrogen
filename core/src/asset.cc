#include "core/asset.h"

#include "core/assets.h"
#include "core/log.h"
#include "imgui.h"

uint32_t h_core::Asset::initFromYaml(const std::string& basePath, const YAML::Node& yaml) {
    //    HYLOG_WARN("ASSET: Undefined initFromYaml method.\n");

    // this->desc = desc;

    return 0;
}
void h_core::Asset::saveToYaml(YAML::Node& yaml) {
    HYLOG_WARN("ASSET: Undefined saveToYaml method, not saving\n");
}

uint32_t h_core::Asset::precompile(h_core::RuntimeSystems* systems) {
    // A precompile() definition isn't necessary in all/many cases
    // ::printf("WARN: ASSET: Undefined precompile method.\n");

    return 0;
}

size_t h_core::Asset::getPackedSize() const {
    return 0;
}

void h_core::Asset::toPacked(uint8_t* _writeHead) {
    HYLOG_WARN("ASSET: Undefined toPacked method, not writing\n");
}

void h_core::Asset::fromPacked(const uint8_t* _readHead) {
    HYLOG_WARN("ASSET: Undefined fromPacked method, not reading\n");
}

void h_core::Asset::doGUI() {
    ImGui::Text("Asset::doGUI not implemented");
}
