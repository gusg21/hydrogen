#include "core/asset.h"

#include "core/assets.h"
#include "core/log.h"
#include "imgui.h"

uint32_t h_core::Asset::initFromYaml(
    h_core::Assets* assets, const h_core::AssetDescription& desc, const YAML::Node& yaml) {
//    HYLOG_WARN("ASSET: Undefined initFromYaml method.\n");

    this->desc = desc;

    return 0;
}
uint32_t h_core::Asset::precompile(h_core::RuntimeSystems* systems) {
    // A precompile() definition isn't necessary in all/many cases
    // ::printf("WARN: ASSET: Undefined precompile method.\n");

    return 0;
}

std::vector<uint8_t>* h_core::Asset::toPacked() {
    HYLOG_WARN("ASSET: Undefined toPacked method, returning empty\n");
    return new std::vector<uint8_t>();
}

void h_core::Asset::fromPacked(const void* packedData, size_t length) {
    HYLOG_WARN("ASSET: Undefined fromPacked method, not loading\n");
}

void h_core::Asset::doGUI() {
    ImGui::Text("Asset::doGUI not implemented");
}
