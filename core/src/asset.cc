#include "core/asset.h"

uint32_t h_core::Asset::initFromYaml(h_core::Assets* assets, h_core::Systems* systems, YAML::Node yaml) {
    ::printf("WARN: ASSET: Undefined import/init method.\n");

    return 1;
}