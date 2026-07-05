# DogEngine

A 2D game engine built with C++ and OpenGL, based on [Hazel Engine](https://github.com/TheCherno/Hazel) by The Cherno.

## Overview

DogEngine is a modular 2D game engine with a batch-optimized renderer, layer-based application architecture, Dear ImGui editor panels, an Entity Component System, scene management, and physics integration. It uses a Premake5 build system targeting Visual Studio 2022 on Windows.

## Features

### Rendering
- **Batch-optimized 2D renderer** — thousands of quads in a single draw call with automatic batching
- **Texture management** — load images from disk (PNG/JPG) with automatic texture slot allocation (32 slots)
- **Shader system** — load from `.glsl` files with `#type vertex` / `#type fragment` markers, or inline strings; uniform caching avoids repeated driver lookups
- **Material system** — shader + uniform values + textures bundled and applied as one step
- **Shader library** — load shaders once and share by name across the engine
- **Transform system** — position, rotation, and scale for every object with 4×4 matrix computation
- **Alpha blending** with configurable draw order
- **Framebuffer system** — abstraction for rendering to textures with multiple attachments and pixel readback
- **Profiling** — Chrome trace-event JSON output for performance analysis

### Application Framework
- **Layer-based architecture** — stackable layers with update, render, and event lifecycle
- **Timestep & delta time** — frame-rate independent movement
- **Smart pointers** — `Ref<T>` (shared) and `Scope<T>` (unique) for automatic memory management
- **Event system** — keyboard, mouse, and window events with dispatch
- **Input polling** — keyboard and mouse state
- **Orthographic camera controller** — WASD pan, scroll zoom, rotation support
- **Window resize handling** — minimizes rendering when window is minimized

### Entity Component System
- **EnTT-based ECS** — high-performance entity-component architecture
- **Entity wrapper** — `AddComponent<T>()`, `GetComponent<T>()`, `HasComponent<T>()`, `RemoveComponent<T>()`
- **Scene management** — entity creation/destruction, scene update loop, camera queries
- **Components:** Transform, SpriteRenderer, CircleRenderer, Camera, RigidBody2D, BoxCollider2D, CircleCollider2D, Tag, ID

### Editor UI
- **Dear ImGui** — debug UI and performance statistics
- **ImGui Docking** — full dockspace layout with dockable panels
- **Scene Serialization** — YAML-based save/load via yaml-cpp
- **ImGuizmo** — transform gizmos for translate/rotate/scale

### Physics
- **Box2D integration** — physics engine for 2D rigid body simulation
- **Collider support** — box and circle colliders with physics properties (density, friction, restitution)

### Build System
- **Premake5** — generates Visual Studio 2022 solution files
- **C++20 standard** — modern C++ features
- **Modular static library** — clean separation between engine and sandbox

## Controls (Sandbox App)

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
│   ├── DogEngine/
│   │   ├── Renderer/      Batch renderer, shaders, textures, camera, framebuffer
│   │   ├── Scene/         ECS, entities, components, scene serialization
│   │   ├── ImGui/         ImGui layer with dockspace integration
│   │   ├── Events/        Event system with dispatch
│   │   ├── Platform/      OpenGL and Windows platform implementations
│   │   └── Debug/         Profiling and instrumentation
├── assets/            Shader files and assets
├── Sandbox/           Example application
├── DogEngine/vendor/  Third-party dependencies
├── premake5.lua       Build configuration
└── GenerateProject.bat
```

## Dependencies

- **GLFW** — Windowing and input
- **GLAD** — OpenGL function loading
- **Dear ImGui** — Debug UI and editor panels
- **ImGuizmo** — 3D transformation gizmos
- **GLM** — Mathematics
- **EnTT** — Entity Component System
- **spdlog** — Logging
- **stb_image** — Image loading (PNG/JPG)
- **yaml-cpp** — YAML serialization
- **Box2D** — 2D physics engine
- **filewatch** — File system change monitoring

## License

See [LICENSE](LICENSE).
