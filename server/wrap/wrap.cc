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
    SDL_Log("INFO: WRAP: Creating engine\n");
    assets = new h_core::Assets();
}

WRAP_DECL void load_project(const char* project_path, const char* assets_base_path) {
    SDL_Log("INFO: WRAP: Loading project name @ %p\n", project_path);
    SDL_Log("INFO: WRAP: Loading project %s\n", project_path);
    SDL_Log("INFO: WRAP: Assets Base Dir %s\n", assets_base_path);
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

WRAP_DECL std::vector<char>* get_packed_asset_from_index(uint32_t asset_index) {
    h_core::Asset* asset = assets->getAssetByIndex<h_core::Asset>(asset_index);
    return asset->toPacked();
}

WRAP_DECL void delete_packed_asset(std::vector<char>* packed_asset) {
    delete packed_asset;
}

WRAP_DECL char get_data_in_packed_asset(std::vector<char>* packed_asset, uint32_t index) {
    return packed_asset->at(index);
}

WRAP_DECL size_t get_length_of_packed_asset(std::vector<char>* packed_asset) {
    return packed_asset->size();
}

}