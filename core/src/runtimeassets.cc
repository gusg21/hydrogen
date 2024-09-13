//
// Created by angus.goucher on 9/8/2024.
//

#include "core/runtimeassets.h"

#include "imgui.h"

#include "core/log.h"
#include "core/project/project.h"
#include "core/theming/guicolors.h"

void h_core::RuntimeAssets::init(const std::string& serverAddress, h_core::RuntimeSystems* systems) {
    curl_global_init(CURL_GLOBAL_ALL);

    m_serverAddress = serverAddress;

    m_netRequestThreadContext.pingServerAddress = serverAddress;
    m_netRequestThreadContext.netRequestThreadAlive = false;
    m_netRequestThread = std::thread { netRequestThreadFunction, &m_netRequestThreadContext };

    m_systems = systems;
}

uint32_t h_core::RuntimeAssets::command_loadAsset(const std::string& arguments, void* data) {
    h_core::RuntimeAssets* assets = (h_core::RuntimeAssets*)data;
    RUNTIMECONSOLE_PARSE_ARGS(arguments, yaml);

    uint32_t assetType = yaml["type"].as<uint32_t>(UINT32_MAX);
    h_core::AssetIndex assetIndex = yaml["index"].as<h_core::AssetIndex>(ASSET_INDEX_BAD);
    std::string assetPath = yaml["path"].as<std::string>("");
    bool isRemote = yaml["remote"].as<bool>(false);

    if (assetType == UINT32_MAX) { return 2; }
    if (assetIndex == ASSET_INDEX_BAD) { return 3; }
    if (assetPath.empty() && !isRemote) { return 4; }

    CALL_TYPED_FUNC_WITH_ASSET_ID(assetType, assets->loadAsset, assetIndex, assetPath, isRemote);
    assets->m_assets[assetIndex]->precompile(assets->m_systems);

    return 0;
}

void h_core::RuntimeAssets::registerCommands(h_core::RuntimeConsole* console) {
    console->newCommandWithHelp(
        "loadAsset", h_core::RuntimeAssets::command_loadAsset, this,
        "{ type: int, index: int, path: string, remote: bool } load an asset of type 'type' into index 'index' from either path 'path' or from the server if 'remote'");
}

void h_core::RuntimeAssets::destroy() {
    m_netRequestThreadContext.netRequestThreadAlive = true;
    m_netRequestThread.join();
}

void h_core::RuntimeAssets::doGUI() {
    if (ImGui::Begin("Runtime Assets Debugger")) {
        ImGui::PushStyleColor(ImGuiCol_ChildBg, hasServerConnection() ? IMGUI_COLOR_GOOD : IMGUI_COLOR_WARN);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4 { 0, 0, 0, 0.5 });
        constexpr float statusAreaHeight = 50;
        ImGui::BeginChild("Server Status", ImVec2 { 0, statusAreaHeight });
        {
            const char* text = hasServerConnection() ? "CONNECTED TO SERVER" : "NOT CONNECTED TO SERVER";
            ImVec2 textSize = ImGui::CalcTextSize(text);
            float windowW = ImGui::GetWindowWidth();
            float windowH = ImGui::GetWindowHeight();
            ImGui::SetCursorPosX(windowW / 2.f - textSize.x / 2.f);
            ImGui::SetCursorPosY(windowH / 2.f - textSize.y / 2.f);
            ImGui::Text(text);
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();

        ImGui::Separator();

        if (ImGui::CollapsingHeader("Recent Net Request Jobs")) {
            ImGui::Indent();
            uint32_t jobIndex = 0;
            for (NetRequestJob job : m_recentJobs) {
                ImGui::PushID(jobIndex);
                std::string headerText =
                    "Request #" + std::to_string(jobIndex) + " (Asset #" + std::to_string(job.assetIndex) + ")";
                if (ImGui::CollapsingHeader(headerText.c_str())) {
                    ImGui::Text("Asset Index: %d", job.assetIndex);
                    ImGui::Text("Asset Type: %d", job.assetType);
                    ImGui::Text("Server Address: %s", job.serverAddress.c_str());
                }
                ImGui::PopID();
                jobIndex++;
            }
            ImGui::Unindent();
        }

        if (ImGui::CollapsingHeader("Asset List")) {
            ImGui::Indent();
            uint32_t assetIndex = 0;
            for (h_core::Asset* asset : m_assets) {
                if (asset != nullptr) {
                    std::string title = "Asset " + std::to_string(assetIndex);
                    if (ImGui::CollapsingHeader(title.c_str())) {
                        ImGui::Indent();
                        asset->doGUI();
                        ImGui::Separator();
                        if (ImGui::Button("Calc Packed Size")) {
                            std::vector<uint8_t>* packed = asset->toPacked();
                            m_packedSizeMap[assetIndex] = packed->size();
                            delete packed;
                        }
                        if (m_packedSizeMap.find(assetIndex) != m_packedSizeMap.end()) {
                            ImGui::SameLine();
                            ImGui::Text("Packed Size: %u", m_packedSizeMap[assetIndex]);
                        }

                        ImGui::Unindent();
                    }
                }
                assetIndex++;
            }
            ImGui::Unindent();
        }
    }
    ImGui::End();
}

void h_core::RuntimeAssets::loadFromProject(const h_core::project::Project* project) {
    for (const h_core::project::ProjectAssetEntry& assetInfo : project->requiredAssets) {
        CALL_TYPED_FUNC_WITH_ASSET_ID(
            assetInfo.typeId, RuntimeAssets::loadAsset, assetInfo.index, assetInfo.assetPath, assetInfo.isRemote);
    }
}

void h_core::RuntimeAssets::precompile() {
    

    for (int assetIndex = 0; assetIndex < ASSETS_MAX_ASSET_COUNT; ++assetIndex) {
        h_core::Asset* asset = m_assets[assetIndex];
        if (asset != nullptr) { asset->precompile(m_systems); }
    }
}

void h_core::RuntimeAssets::flushAndPrecompileNetAssets() {
    m_netRequestThreadContext.resultQueueLock.lock();

    while (!m_netRequestThreadContext.results.empty()) {
        h_core::NetRequestResult result = m_netRequestThreadContext.results.front();

        if (result.success) {
            // Replace old asset
            if (m_assets[result.job.assetIndex] != nullptr) { delete m_assets[result.job.assetIndex]; }
            m_assets[result.job.assetIndex] = result.newAsset;
            result.newAsset->precompile(m_systems);
        }
        else {
            // Re-queue new asset request
            CALL_TYPED_FUNC_WITH_ASSET_ID(result.job.assetType, RuntimeAssets::requestNetAsset, result.job.assetIndex);
        }

        m_netRequestThreadContext.results.pop_front();
    }

    m_netRequestThreadContext.resultQueueLock.unlock();
}

void h_core::RuntimeAssets::netRequestThreadFunction(h_core::NetRequestThreadContext* context) {
    HYLOG_INFO("ASSETS: Net request thread running!\n");

    while (!context->netRequestThreadAlive) {
        // Do jobs if needed
        if (!context->jobs.empty() && context->hasServerConnection) {
            // Request asset
            NetRequestJob job = context->jobs.front();
            HYLOG_INFO("ASSETS: THREAD: got job request for asset %d\n", job.assetIndex);
            h_core::Asset* newAsset = nullptr;
            CURLcode result;
            CALL_TYPED_FUNC_WITH_ASSET_ID(
                job.assetType, h_core::RuntimeAssets::requestNetAssetNow, &newAsset, &result, job.serverAddress,
                job.assetIndex);
            HYLOG_INFO("ASSETS: THREAD: completed net request for asset %d\n", job.assetIndex);

            // Apply to asset list
            HYLOG_INFO("ASSETS: THREAD: awaiting asset list access... \n", job.assetIndex);
            context->resultQueueLock.lock();
            {
                // Add result to the return list
                if (newAsset == nullptr) { context->results.emplace_back(nullptr, job, false); }
                else { context->results.emplace_back(newAsset, job, true); }
            }
            context->resultQueueLock.unlock();
            HYLOG_INFO("ASSETS: THREAD: updated asset list \n", job.assetIndex);

            // remove job
            context->jobs.pop_front();
        }

        // Update connection test
        CURL* connectTest = curl_easy_init();
        curl_easy_setopt(connectTest, CURLOPT_URL, context->pingServerAddress.c_str());
        curl_easy_setopt(connectTest, CURLOPT_CONNECT_ONLY, 1L);
        CURLcode result = curl_easy_perform(connectTest);
        if (result != CURLE_OK && result != CURLE_COULDNT_CONNECT) {
            HYLOG_WARN("ASSETS: Ping attempt returned curl error code %d\n", result);
        }
        context->hasServerConnection = result == CURLE_OK;
    }

    HYLOG_INFO("ASSETS: Killing net request thread...\n");
}

bool h_core::RuntimeAssets::hasServerConnection() {
    return m_netRequestThreadContext.hasServerConnection;
}