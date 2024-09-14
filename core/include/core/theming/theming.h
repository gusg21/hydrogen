#include "imgui.h"

namespace h_core {
namespace theming {
void editor() {
    // cherry colors, 3 intensities
#define HI_EDITOR(v)  ImVec4(50.f / 255.f, 176.f / 255.f, 90.f / 255.f, v)
#define MED_EDITOR(v) ImVec4(48.f / 255.f, 140.f / 255.f, 55.f / 255.f, v)
#define LOW_EDITOR(v) ImVec4(25.f / 255.f, 46.f / 255.f, 22.f / 255.f, v)
    // backgrounds (@todo: complete with BG_MED, BG_LOW)
#define BG_EDITOR(v) ImVec4(0.220f, 0.270f, 0.200f, v)
    // text
#define TEXT_EDITOR(v) ImVec4(0.860f, 0.930f, 0.890f, v)

    auto& style = ImGui::GetStyle();

    style.Colors[ImGuiCol_Text] = TEXT_EDITOR(0.78f);
    style.Colors[ImGuiCol_TextDisabled] = TEXT_EDITOR(0.28f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_ChildBg] = BG_EDITOR(0.58f);
    style.Colors[ImGuiCol_PopupBg] = BG_EDITOR(0.9f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.539f, 0.479f, 0.255f, 0.162f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = BG_EDITOR(1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = MED_EDITOR(0.78f);
    style.Colors[ImGuiCol_FrameBgActive] = MED_EDITOR(1.00f);
    style.Colors[ImGuiCol_TitleBg] = LOW_EDITOR(1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = HI_EDITOR(1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = BG_EDITOR(0.75f);
    style.Colors[ImGuiCol_MenuBarBg] = BG_EDITOR(0.47f);
    style.Colors[ImGuiCol_ScrollbarBg] = BG_EDITOR(1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = MED_EDITOR(0.78f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = MED_EDITOR(1.00f);
    style.Colors[ImGuiCol_CheckMark] = HI_EDITOR(1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
    style.Colors[ImGuiCol_ButtonHovered] = MED_EDITOR(0.86f);
    style.Colors[ImGuiCol_ButtonActive] = MED_EDITOR(1.00f);
    style.Colors[ImGuiCol_Header] = MED_EDITOR(0.76f);
    style.Colors[ImGuiCol_HeaderHovered] = MED_EDITOR(0.86f);
    style.Colors[ImGuiCol_HeaderActive] = HI_EDITOR(1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
    style.Colors[ImGuiCol_ResizeGripHovered] = MED_EDITOR(0.78f);
    style.Colors[ImGuiCol_ResizeGripActive] = MED_EDITOR(1.00f);
    style.Colors[ImGuiCol_PlotLines] = TEXT_EDITOR(0.63f);
    style.Colors[ImGuiCol_PlotLinesHovered] = MED_EDITOR(1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = TEXT_EDITOR(0.63f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = MED_EDITOR(1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = MED_EDITOR(0.43f);

    style.WindowPadding = ImVec2(8, 8);
    style.WindowRounding = 10.0f;
    style.FrameRounding = 3.0f;
    // style.ItemSpacing = ImVec2(7, 1);
    // style.ItemInnerSpacing = ImVec2(1, 1);
    style.TouchExtraPadding = ImVec2(0, 0);
    //    style.IndentSpacing = 6.0f;
    style.ScrollbarSize = 12.0f;
    style.ScrollbarRounding = 16.0f;
    style.GrabMinSize = 20.0f;
    style.GrabRounding = 2.0f;

    style.WindowTitleAlign.x = 0.50f;

    // style.FrameBorderSize = 0.0f;
    // style.WindowBorderSize = 1.0f;
}


void runtime() {
    // cherry colors, 3 intensities
#define HI_RUNTIME(v)  ImVec4(86.f / 255.f, 86.f / 255.f, 234.f / 255.f, v)
#define MED_RUNTIME(v) ImVec4(47.f / 255.f, 46.f / 255.f, 204.f / 255.f, v)
#define LOW_RUNTIME(v) ImVec4(12.f / 255.f, 5.f / 255.f, 218.f / 255.f, v)
    // backgrounds (@todo: complete with BG_MED, BG_LOW)
#define BG_RUNTIME(v) ImVec4(0.200f, 0.220f, 0.270f, v)
    // text
#define TEXT_RUNTIME(v) ImVec4(0.860f, 0.930f, 0.890f, v)

    auto& style = ImGui::GetStyle();

    style.Colors[ImGuiCol_Text] = TEXT_RUNTIME(0.78f);
    style.Colors[ImGuiCol_TextDisabled] = TEXT_RUNTIME(0.28f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_ChildBg] = BG_RUNTIME(0.58f);
    style.Colors[ImGuiCol_PopupBg] = BG_RUNTIME(0.9f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.539f, 0.479f, 0.255f, 0.162f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = BG_RUNTIME(1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = MED_RUNTIME(0.78f);
    style.Colors[ImGuiCol_FrameBgActive] = MED_RUNTIME(1.00f);
    style.Colors[ImGuiCol_TitleBg] = LOW_RUNTIME(1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = HI_RUNTIME(1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = BG_RUNTIME(0.75f);
    style.Colors[ImGuiCol_MenuBarBg] = BG_RUNTIME(0.47f);
    style.Colors[ImGuiCol_ScrollbarBg] = BG_RUNTIME(1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = MED_RUNTIME(0.78f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = MED_RUNTIME(1.00f);
    style.Colors[ImGuiCol_CheckMark] = HI_RUNTIME(1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
    style.Colors[ImGuiCol_ButtonHovered] = MED_RUNTIME(0.86f);
    style.Colors[ImGuiCol_ButtonActive] = MED_RUNTIME(1.00f);
    style.Colors[ImGuiCol_Header] = MED_RUNTIME(0.76f);
    style.Colors[ImGuiCol_HeaderHovered] = MED_RUNTIME(0.86f);
    style.Colors[ImGuiCol_HeaderActive] = HI_RUNTIME(1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
    style.Colors[ImGuiCol_ResizeGripHovered] = MED_RUNTIME(0.78f);
    style.Colors[ImGuiCol_ResizeGripActive] = MED_RUNTIME(1.00f);
    style.Colors[ImGuiCol_PlotLines] = TEXT_RUNTIME(0.63f);
    style.Colors[ImGuiCol_PlotLinesHovered] = MED_RUNTIME(1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = TEXT_RUNTIME(0.63f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = MED_RUNTIME(1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = MED_RUNTIME(0.43f);

    style.WindowPadding = ImVec2(8, 8);
    style.WindowRounding = 10.0f;
    style.FrameRounding = 3.0f;
    // style.ItemSpacing = ImVec2(7, 1);
    // style.ItemInnerSpacing = ImVec2(1, 1);
    style.TouchExtraPadding = ImVec2(0, 0);
    //    style.IndentSpacing = 6.0f;
    style.ScrollbarSize = 12.0f;
    style.ScrollbarRounding = 16.0f;
    style.GrabMinSize = 20.0f;
    style.GrabRounding = 2.0f;

    style.WindowTitleAlign.x = 0.50f;

    // style.FrameBorderSize = 0.0f;
    // style.WindowBorderSize = 1.0f;
}
}  // namespace theming

}  // namespace h_core