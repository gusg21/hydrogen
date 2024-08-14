#include "core/assets.h"

h_core::AssetHash h_core::Assets::getAssetHashFromString(
    tinystl::string string) {
    return tinystl::hash_string(string.c_str(), string.size());
}