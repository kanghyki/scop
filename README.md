# SCOP

**SCOP** is a lightweight OpenGL-based 3D object viewer. It allows users to load `.obj` files, apply textures, and interact with the model in a 3D space.

## Prerequisites & Environment

This project is designed to be built using **CMake** and **Make**.

### Supported Platforms

- **macOS** (Tested on Darwin)
- **Linux** (Requires OpenGL/X11 libraries)

### Requirements

- **C++ Compiler** (clang++ or g++) supporting C++11 or higher
- **CMake** (3.10 or higher)
- **Make**

## Building the Project

The project includes a `Makefile` that wraps the CMake build process for convenience.

1.  **Clone the repository** (if not already done).
2.  **Build the project**:

    ```bash
    make
    ```

    This command creates a `build/` directory, compiles dependencies (GLFW, GLAD), and links the `scop` executable.

3.  **Clean the build** (optional):

    ```bash
    make clean
    ```

4.  **Rebuild** (optional):
    ```bash
    make re
    ```

## Running the Application

The application requires a configuration file (`.scop`) to specify the shaders, texture, and 3D model.

### Using Make (Recommended)

You can run the project using the `run` rule and specifying the config file via the `CONFIG` variable:

```bash
# Run with example
make run CONFIG=resource/config/teapot.scop
```

### Manual Execution

Alternatively, you can run the executable directly from the `build/bin` directory:

```bash
./build/bin/scop resource/config/teapot.scop
```

---

## Controls

| Key           | Action                                |
| :------------ | :------------------------------------ |
| **W / S**     | Move Object Z-axis (Forward/Backward) |
| **A / D**     | Move Object X-axis (Left/Right)       |
| **Q / E**     | Move Object Y-axis (Up/Down)          |
| **SPACE**     | Toggle Texture Mode                   |
| **Left CTRL** | Hold for Wireframe Mode               |
| **ESC**       | Exit Application                      |

---

## Configuration File (.scop)

You can create custom configuration files. The format expects `key=value` pairs:

```ini
VS=opengl/shader/glsl.vs       # Path to Vertex Shader
FS=opengl/shader/glsl.fs       # Path to Fragment Shader
TEX=resource/tga/table.tga     # [Optional] Path to Texture (TGA)
OBJ=resource/teapot.obj        # Path to Wavefront OBJ file
```

If `TEX` is missing or the file cannot be loaded, the application will start in the default color mode.
