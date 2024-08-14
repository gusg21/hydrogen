# hydrogen

A small experimental game engine by Xavier Olmstead (@XavOlmst) and Angus Goucher (@gusg21).

Using [Orthodox C++](https://gist.github.com/bkaradzic/2e39896bc7d8c34e042b), [bgfx](https://github.com/bkaradzic/bgfx), [SDL3](https://github.com/libsdl-org/SDL), and [imgui](https://github.com/ocornut/imgui).

### Project Structure

`core` - core library + components

`editor` - editor interface for editing projects (requires `core`)

`runtime` - runtime for running projects (requires `core`)

### Code Style

Code style is rigorously defined by the `.clang-format` file, use clang-format for formatting! It's built into VS, and all you need to do is download LLVM for VS Code.

### Naming

- `ClassName`
  - `I_InterfaceName`
- `functionName()`
- `m_privateVariable`
- `publicVariable`
- `localVariable`
- `h_[core/runtime/editor]::namespacename`

Use these action/inverse action name pairs:

- start/stop
- begin/end
- head/tail

_(Don't mix and match!)_
