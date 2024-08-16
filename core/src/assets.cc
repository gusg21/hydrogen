#include "core/assets.h"

#include <string>

h_core::AssetHash h_core::Assets::getAssetHashFromString(std::string string) {
    return std::hash<std::string>()(string);
}

