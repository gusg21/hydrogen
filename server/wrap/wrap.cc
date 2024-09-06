#include "core/serverengine.h"

#if __WIN32__
#define WRAP_DECL __declspec(dllexport)
#else
#define WRAP_DECL
#endif

h_core::ServerEngine* engine;

extern "C" {

WRAP_DECL void create_engine() {
    SDL_Log("INFO: WRAP: Creating engine\n");
    engine = new h_core::ServerEngine();
}

WRAP_DECL void load_project(const char* project_path, const char* assets_base_path) {
    SDL_Log("INFO: WRAP: Loading project name @ %p\n", project_path);
    SDL_Log("INFO: WRAP: Loading project %s\n", project_path);
    SDL_Log("INFO: WRAP: Assets Base Dir %s\n", assets_base_path);
    h_core::project::Project project {};
    project.loadFromFile(project_path, assets_base_path);
    engine->init(&project);
}

WRAP_DECL std::vector<char>* get_packed_asset_from_index(uint32_t asset_index) {
    h_core::Asset* asset = engine->getAssets()->getAssetByIndex<h_core::Asset>(asset_index);
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