#include "core/engine.h"

#define STB_IMAGE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION
#include "tiny_gltf.h"

#include "core/systems/sys_gravity.h"
#include "core/systems/renderer/renderer.h"
#include "core/theming/theming.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"

uint32_t h_core::Engine::init(h_core::Project* project) {
    m_systems[0] = new h_core::systems::Gravity();

    m_project = project;

    std::string windowTitle = "hydrogen runtime - " + project->projectName;

    m_window = new h_core::Window();
    uint32_t windowInitResult = m_window->init(
        windowTitle, project->windowWidth, project->windowHeight, false);
    if (windowInitResult != 0) { return ENGINE_INIT_FAIL_BAD_WINDOW_INIT; }
    m_systems[1] = m_window->getRendererSystem();

    m_windowWidth = project->windowWidth;
    m_windowHeight = project->windowHeight;

    // ImGui setup
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;

    h_core::theming::cherry();
    // ImGui::StyleColorsLight();

    ImGui_ImplOpenGL3_Init();
    ImGui_ImplSDL2_InitForOpenGL(
        m_window->getSDLWindow(),
        m_window->getRendererSystem()->getGLContext());

    for (uint32_t systemIndex = 0; systemIndex < ENGINE_SYSTEM_COUNT;
         systemIndex++) {
        m_systems[systemIndex]->engine = this;
        if (m_systems[systemIndex]->init() != 0) {
            printf("ERROR: ENGINE: Failed to init system index %d\n", systemIndex);
            return ENGINE_INIT_FAIL_BAD_SYSTEM_INIT;
        }
    }

    return 0;
}

void h_core::Engine::destroy() {
    for (uint32_t systemIndex = 0; systemIndex < ENGINE_SYSTEM_COUNT;
         systemIndex++) {
        // Call it to tell it it's going to die
        m_systems[systemIndex]->destroy();

        // Shoot it
        delete m_systems[systemIndex];

        // Bury it and erase its memory
        m_systems[systemIndex] = nullptr;

        // (ﾉ◕ヮ◕)ﾉ*:･ﾟ✧
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    m_window->destroy();
    delete m_window;
}

void h_core::Engine::run() {
    // Set up first scene
    if (m_project->initialSceneSpec != ASSETS_ASSET_INDEX_BAD) {
        m_scene.initFromSceneSpecAssetIndex(
            &m_project->assets, m_project->initialSceneSpec);
    }

    bool engineRunning = true;
    while (engineRunning) {
        m_window->postEventsToQueue(&m_events);

        for (uint32_t eventIndex = 0; eventIndex < m_events.getSize();
             eventIndex++) {
            h_core::Event event = m_events.getHeadPointer()[eventIndex];
            switch (event.type) {
                case ENGINE_EVENT_QUIT:
                    engineRunning = false;
                    break;
                case ENGINE_EVENT_RESIZED:
                    printf("INFO: ENGINE: Wow!\n");
                    m_windowWidth = event.newWindowWidth;
                    m_windowHeight = event.newWindowHeight;
                    break;
                default:
                    break;
            }
        }

        // Begin ImGui Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // Demo window
        ImGui::ShowDemoWindow();

        // Begin drawing for every system
        for (uint32_t systemIndex = 0; systemIndex < ENGINE_SYSTEM_COUNT;
             systemIndex++) {
            m_systems[systemIndex]->beginFrame();
        }

        // Update all actors
        for (uint32_t systemIndex = 0; systemIndex < ENGINE_SYSTEM_COUNT;
             systemIndex++) {
            m_scene.processSystem(m_systems[systemIndex]);
        }

        // Draw the actors
        for (uint32_t systemIndex = 0; systemIndex < ENGINE_SYSTEM_COUNT;
             systemIndex++) {
            m_scene.drawSystem(m_systems[systemIndex]);
        }

        // End the frame for every system
        for (uint32_t systemIndex = 0; systemIndex < ENGINE_SYSTEM_COUNT;
             systemIndex++) {
            m_systems[systemIndex]->endFrame();
        }

        // Send ImGui draw data
        ImGui::Render();
        // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update the window
        m_window->swap();

        // Clear the queue
        m_events.clear();
    }
}


uint32_t h_core::Engine::getWidth() {
    return m_windowWidth;
}

uint32_t h_core::Engine::getHeight() {
    return m_windowHeight;
}

h_core::math::Color h_core::Engine::getClearColor() {
    return m_clearColor;
}