#include <stdio.h>

#include "core/window.h"

int main() {
    h_core::Window* window = new h_core::Window("", 1600, 900, false);

    printf("Editor built successfully!\n");
}