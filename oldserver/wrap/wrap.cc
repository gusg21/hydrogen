#include "core/assets.h"
#include "core/project/project.h"

#if __WIN32__
#define WRAP_DECL __declspec(dllexport)
#else
#define WRAP_DECL
#endif

h_core::Assets* assets;

extern "C" {

WRAP_DECL void create_engine() {
    HYLOG_INFO("WRAP: Creating engine\n");
    assets = new h_core::Assets();
}

WRAP_DECL void load_project(const char* project_path, const char* assets_base_path) {
    HYLOG_INFO("WRAP: Loading project name @ %p\n", project_path);
    HYLOG_INFO("WRAP: Loading project %s\n", project_path);
    HYLOG_INFO("WRAP: Assets Base Dir %s\n", assets_base_path);
    h_core::project::Project project {};
    project.loadFromFile(project_path, assets_base_path);
    assets->loadFromProject(&project);
}

WRAP_DECL uint32_t get_loaded_asset_count() {
    return assets->getAssetCount();
}

WRAP_DECL uint32_t get_max_asset_count() {
    return ASSETS_MAX_ASSET_COUNT;
}

WRAP_DECL uint8_t* get_packed_asset_from_index(uint32_t asset_index) {
    h_core::Asset* asset = assets->getAssetByIndex<h_core::Asset>(asset_index);
    HYLOG_DEBUG("WRAP: Got asset pointer for index %d", asset_index);
    uint8_t* packed = new uint8_t[asset->getPackedSize()];
    asset->toPacked(packed);
    return packed;
}

WRAP_DECL bool is_packed_asset_index_valid(uint32_t asset_index) {
    h_core::Asset* asset = assets->getAssetByIndex<h_core::Asset>(asset_index);
    return asset != nullptr;
}

WRAP_DECL void delete_packed_asset(uint8_t* packed_asset) {
    delete packed_asset;
}

WRAP_DECL uint8_t get_data_in_packed_asset(uint8_t* packed_asset, uint32_t index) {
    return packed_asset[index];
}

WRAP_DECL size_t get_length_of_packed_asset(uint32_t asset_index) {
    return assets->getAssetByIndex<h_core::Asset>(asset_index)->getPackedSize();
}

}