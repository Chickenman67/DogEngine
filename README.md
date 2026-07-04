# DogEngine

A 2D game engine built with C++ and OpenGL, based on [Hazel Engine](https://github.com/TheCherno/Hazel) by The Cherno.

## Features

- **OpenGL rendering** with batch-optimized 2D renderer
- **Layer-based architecture** — stackable layers with update, render, and event lifecycle
- **Orthographic camera** with WASD pan, scroll zoom, and rotation
- **Batch rendering** — thousands of quads in a single draw call
- **Texture support** — load images from disk with automatic slot management
- **Shader system** — load from `.glsl` files or inline strings with uniform caching
- **Material system** — shader + uniform values + textures bundled together
- **Shader library** — load once, share by name across the engine
- **Alpha blending** with configurable draw order
- **Transform system** — position, rotation, and scale for every object
- **Timestep & delta time** — frame-rate independent movement
- **Smart pointers** — `Ref<T>` (shared) and `Scope<T>` (unique) for automatic memory management
- **Event system** — keyboard, mouse, and window events with dispatch
- **Input polling** — static API for keyboard and mouse state
- **Dear ImGui integration** — debug UI and performance stats
- **Profiling & instrumentation** — Chrome trace-event JSON output for performance analysis
- **Premake5 build system** — generates Visual Studio 2022 solutions

## Controls

| Key | Action |
|-----|--------|
| W / S | Move camera up / down |
| A / D | Move camera left / right |
| Scroll | Zoom in / out |
| F11 | Toggle fullscreen |

## Building

1. Run `GenerateProject.bat` to generate the Visual Studio solution
2. Open `DogEngine.sln`
3. Build and run the **Sandbox** project

## Project Structure

```
DogEngine/
├── src/               Engine source code
├── assets/            Shader files and assets
├── Sandbox/           Example application
├── DogEngine/vendor/  Third-party dependencies
├── premake5.lua       Build configuration
└── GenerateProject.bat
```

## Dependencies

- **GLFW** — Windowing and input
- **GLAD** — OpenGL function loading
- **Dear ImGui** — Debug UI
- **GLM** — Mathematics
- **spdlog** — Logging
- **stb_image** — Image loading

## License

See [LICENSE](LICENSE).
