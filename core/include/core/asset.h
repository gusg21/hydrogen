#pragma once

#include <tinystl/string.h>

namespace h_core {
class Asset {
  public:
    void loadFromYaml();

    /// @brief the name of the asset
    tinystl::string name {};
};
}  // namespace h_core