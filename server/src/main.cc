//
// Created by Angus Goucher on 9/21/2024.
//

#include "core/asset.h"
#include "core/assets.h"
#include "core/project/project.h"
#include "httplib.h"

int main(int argc, char* args[]) {
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

    // Load assets
    h_core::Assets assets {};
    h_core::project::Project project {};
    project.loadFromFile("assets/project.hyproject", SDL_GetBasePath());
    assets.loadFromProject(&project);

    // Configure server
    httplib::Server svr;
    svr.Get("/", [&](const httplib::Request& req, httplib::Response& res) {
        res.set_content("hydrogen asset server is alive", "text/plain");
    });

    svr.Get("/asset/:id", [&](const httplib::Request& req, httplib::Response& res) {
        std::string assetIndexStr = req.path_params.at("id");

        h_core::AssetIndex assetIndex;
        try {
            assetIndex = std::stoi(assetIndexStr);
        } catch (const std::invalid_argument& e) {
            HYLOG_WARN("SERVER: Failed to parse asset index string %s", assetIndexStr.c_str());
            res.set_content("bad asset index", "text/plain");
            res.status = 404;
            return;
        }

        HYLOG_INFO("SERVER: Serving asset index %d...", assetIndex);

        h_core::Asset* asset = assets.getAssetByIndex<h_core::Asset>(assetIndex);
        if (asset == nullptr) {
            HYLOG_WARN("SERVER: asset index %d does not exist", assetIndex);
            res.set_content("nonexistent asset", "text/plain");
            res.status = 404;
            return;
        }

        size_t packedSize = asset->getPackedSize();
        uint8_t* packed = new uint8_t[packedSize];
        asset->toPacked(packed);

        res.set_content((char*)packed, packedSize, "bin/hy");

        HYLOG_INFO("SERVER: Done serving asset index %d...", assetIndex);
    });

    // Listen
    const char* host = "localhost";
    uint32_t port = 5000;
    HYLOG_INFO("SERVER: Listening on %s:%d...", host, port);
    svr.listen(host, port);

    return 0;
}