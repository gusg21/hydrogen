#include "core/engine.h"

#define STB_IMAGE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION
#include "tiny_gltf.h"

#include "core/theming/theming.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"

uint32_t h_core::Engine::init(
    h_core::Assets* out_assets, h_core::Project* project) {
    // Store the project
    m_project = project;

    // Window initialization
    m_window = new h_core::Window();
    std::string windowTitle = "hydrogen runtime - " + project->name;
    uint32_t windowInitResult = m_window->init(
        windowTitle, project->windowWidth, project->windowHeight, false);
    if (windowInitResult != 0) { return ENGINE_INIT_FAIL_BAD_WINDOW_INIT; }
    m_windowWidth = project->windowWidth;
    m_windowHeight = project->windowHeight;

    // ImGui setup
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;

    ::ImGui_ImplOpenGL3_Init();
    ::ImGui_ImplSDL2_InitForOpenGL(
        m_window->getSDLWindow(), m_window->getGLContext());
    h_core::theming::cherry();

    doInit();

    // load the assets
    m_assets = out_assets;
    m_assets->loadFromProject(project);

    doPostLoad();

    return 0;
}

void h_core::Engine::destroy() {
    ::ImGui_ImplOpenGL3_Shutdown();
    ::ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    m_window->destroy();
    delete m_window;

    // (ﾉ◕ヮ◕)ﾉ*:･ﾟ✧
}

void h_core::Engine::run() {
    // Set up first scene

    prepareScene(m_project->initialSceneSpec);
    //    // Prepare the systems for this scene
    //    m_systems.initScene(&m_scene);

    // Loop
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
                    ::printf("INFO: ENGINE: Wow!\n");
                    m_windowWidth = event.newWindowWidth;
                    m_windowHeight = event.newWindowHeight;
                    break;
                default:
                    break;
            }
        }

        // Begin ImGui Frame
        ::ImGui_ImplOpenGL3_NewFrame();
        ::ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // Make the game happen!
        doGUI();
        beginFrame();
        doProcess();
        doDraw();
        endFrame();

        //        // Demo window
        //        ImGui::ShowDemoWindow();
        //
        //        m_systems.beginFrame();
        //        m_systems.processScene(&m_scene);
        //        m_systems.drawScene(&m_scene);
        //        m_systems.endFrame();

        // Send ImGui draw data
        ImGui::Render();
        ::ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update the window
        m_window->swap();

        // Clear the queue
        m_events.clear();
    }
}

uint32_t h_core::Engine::getWidth() const {
    return m_windowWidth;
}

uint32_t h_core::Engine::getHeight() const {
    return m_windowHeight;
}

h_core::math::Color h_core::Engine::getClearColor() const {
    return m_clearColor;
}

h_core::Scene* h_core::Engine::getScene() {
    return &m_scene;
}

h_core::Window* h_core::Engine::getWindow() {
    return m_window;
}

h_core::Assets* h_core::Engine::getAssets() {
    return m_assets;
}
const h_core::Project* h_core::Engine::getProject() {
    return m_project;
}
