#include <stdint.h>
#include <SDL3/SDL.h>

namespace h_core
{
    class Window {
    public:
        Window(const char* title, uint32_t width, uint32_t height, bool fullscreen);
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

    private:
        SDL_Window* m_window;
    };
} // namespace h_core
