# hydrogen

A small experimental game engine by Xavier Olmstead (@XavOlmst) and Angus Goucher (@gusg21).

* C++
* OpenGL 4.1
* [glad](https://glad.dav1d.de/)
* [imgui](https://github.com/ocornut/imgui)
* [SDL2](https://www.libsdl.org/)
* [tinygltf](https://github.com/syoyo/tinygltf)
* [yaml-cpp](https://github.com/jbeder/yaml-cpp)

### Project Structure

`core` - core library + components

`editor` - editor interface for editing projects (requires `core`)

`runtime` - runtime for running projects (requires `core`)

### Code Style

Code style is rigorously defined by the `.clang-format` file, use clang-format for formatting! It's built into VS, and all you need to do is download LLVM for VS Code.

### Coding Guidelines

* Return error values, don't throw exceptions
  * 0 always means success, any other values (positive or negative) indicate failure. Check the header to see which constants represent which errors
* Constructors should be minimal, especially if they can fail
  * (Prefer `::init()` functions instead)

### Naming

- `ClassName`
  - `I_InterfaceName`
- `functionName()`
- `m_privateVariable`
- `publicVariable`
- `localVariable`
- `out_outputParameter`
- `h_[core/runtime/editor]::namespacename`
