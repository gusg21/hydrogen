#include "core/asset.h"

uint32_t h_core::Asset::initFromYaml(
    h_core::Assets* assets, YAML::Node yaml) {
    ::printf("WARN: ASSET: Undefined initFromYaml method.\n");

    return 1;
}
uint32_t h_core::Asset::precompile(h_core::Systems* systems) {
    // A precompile() definition isn't necessary in all/many cases
    // ::printf("WARN: ASSET: Undefined precompile method.\n");

    return 0;
}
