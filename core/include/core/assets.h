#pragma once

#include <stdint.h>

#include "tinystl/string.h"
#include "tinystl/unordered_map.h"
#include "tinystl/vector.h"

#include "core/asset.h"

namespace h_core {
typedef uint32_t AssetHash;

// Base class for all sources of assets (packed/unpacked)
class Assets {
  public:
    Assets() = default;

    /// @brief convert the name of an asset to its hash
    /// @param string the string to convert (asset name)
    /// @return the hash
    static AssetHash getAssetHashFromString(tinystl::string string);

  private:
    tinystl::vector<Asset> m_assets {};
    tinystl::unordered_map<AssetHash, uint32_t>
        m_assetIndexLut {};  // hash -> index
};
}  // namespace h_core