#include <stdint.h>

namespace h_core
{
    class Window {
    public:
        Window(const char* title, uint32_t width, uint32_t height);
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

    private:
        
    };
} // namespace h_core
