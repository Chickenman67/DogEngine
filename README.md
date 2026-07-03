# DogEngine

A 2D game engine built with C++ and OpenGL, based on the super famous [GodEngine](https://github.com/Chickenman67/GodEngine) by [TheGoat](https://github.com/Chickenman67) and [Hazel Engine](https://github.com/TheCherno/Hazel) by The Cherno.

## Features

- OpenGL rendering with GLAD and GLFW
- Orthographic camera system with pan, zoom, and rotation
- Layer-based application architecture
- Input handling (keyboard and mouse)
- ImGui debug UI integration
- Premake5 build system
- GLM math library for vector/matrix operations
- Event system (window close, key press, mouse events)
- Sandbox application for testing

## Controls

- Arrow Keys — Move camera (up/down/left/right)
- A / D — Rotate camera left/right
- F11 — Toggle fullscreen

## Building

Run `GenerateProject.bat` to generate Visual Studio solution files, then open `DogEngine.sln` and build.

## Dependencies

- GLFW — Windowing and input
- GLAD — OpenGL function loading
- ImGui — Debug UI
- GLM — Mathematics
- spdlog — Logging
