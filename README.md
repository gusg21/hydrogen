# hydrogen 

A small experimental game engine by Xavier Olmstead (@XavOlmst) and Angus Goucher (@gusg21).

* C++
* OpenGL 4.1
* [glad](https://glad.dav1d.de/)
* [imgui](https://github.com/ocornut/imgui)
* [SDL2](https://www.libsdl.org/)
* [tinygltf](https://github.com/syoyo/tinygltf)
* [yaml-cpp](https://github.com/jbeder/yaml-cpp)

`dev` status: ![dev status](https://github.com/gusg21/hydrogen/actions/workflows/autobuild.yml/badge.svg?branch=dev) 

`main` status: ![main status](https://github.com/gusg21/hydrogen/actions/workflows/autobuild.yml/badge.svg?branch=main)

### Project Structure

`core` - core library + components

`editor` - editor interface for editing projects (requires `core`)

`runtime` - runtime for running projects (requires `core`)

`server` - server for hosting assets (not multiplayer) (requires `core`)

### Code Style

Code style is rigorously defined by the `.clang-format` file, use clang-format for formatting! It's built into VS, and all you need to do is download LLVM for VS Code.

### Coding Guidelines

See the guidelines [here](https://github.com/gusg21/hydrogen/wiki/Code-Guidelines).

### Naming

- `ClassName`
  - `I_InterfaceName`
- `functionName()`
- `m_privateVariable`
- `publicVariable`
- `localVariable`
- `out_outputParameter`
- `h_[core/runtime/editor]::namespacename`
