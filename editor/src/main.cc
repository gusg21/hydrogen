#include <stdio.h>
#include <string>

#include "core/window.h"

int main() {
    h_core::Window* window = new h_core::Window();
    window->init("", 1600, 900, false);

    printf("Editor built successfully!\n");

    return 0;
}