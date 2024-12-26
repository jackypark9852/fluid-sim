# Fluid Simulator Project

## **Environment Setup**
- **OpenGL Integration**:
  - Import `<glad/glad.h>` and `<GLFW/glfw3.h>` for OpenGL functionality.
- **GLM Integration**:
  - Import `"glm_includes.h"`. This header conveniently includes multiple GLM headers.
- **Project Structure**:
  - Header files are located in `$(ProjectDir)\includes`.
  - Source files are located in `$(ProjectDir)\src`.
- **Adding New Files**:
  1. Right-click the project in the Solution Explorer.
  2. Select "Add Item."
  3. Ensure the files are added to the correct directories (`includes` for headers and `src` for source files).

---

## **Current Features**
- **Quad Rendering**:
  - Full-screen quad rendering from a texture.
- **ImGui Integration**:
  - Fully integrated ImGui interface.
- **Fluid Simulator Header**:
  - Header file for the fluid simulator based on the reference paper.

---

## **To-Do**
1. **Density Step Implementation**:
   - Complete the function to simulate the density step.
2. **Velocity Step**:
   - Declare and implement functions to handle the velocity step.
3. **Texture Conversion**:
   - Write a function to convert the density and velocity fields into a renderable texture.

---

## **Notes**
- **Memory Management**:
  - `std::vector<float>` is used instead of raw pointers (`float*`) to avoid manual memory management.
- **Main Loop Integration**:
  - Call `FluidSimulator::Tick()` from the main update loop.
- **Time Management**:
  - `FluidSimulator` can internally use a clock to determine the time delta (`dt`) between `Tick()` calls.
- Sorry I didn't get as far as I hoped, but good luck and thanks y'all!
