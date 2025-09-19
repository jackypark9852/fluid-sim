# Fluid Simulator Project
Based on Jos Stam's paper, *Real-Time Fluid Dynamics for Games*.

For a quick video showcase, please see https://vimeo.com/1102785370?fl=pl&fe=vl

## **Current Features**
- **Quad Rendering**:
  - Full-screen quad rendering from a texture.
- **ImGui Integration**:
  - Fully integrated ImGui interface.
- **Fluid Simulator Header**:
  - Header file for the fluid simulator based on the reference paper.
- **Density and Velocity Step**
  - Two-step fluid simulation.
- **Texture Conversion**
  - Velocity and Density fields are converted into renderable textures.
  - The density field is rendered, and users can display the velocity field texture by holding the C key.
- **Density Injection**
  - Users can add density in real time by holding the left mouse button.
- **Velocity Injection**
  - Users can add velocity by dragging their mouse in the desired direction while holding the right mouse button.
  - The distance dragged increases the injected velocity's magnitude.
- **Velocity Vector Field**
  - The velocity vector field can be visualized by holding the V key.
  - The magnitude of the arrows shows the magnitude of the velocity at that grid point.
  - Arrows are instance rendered and placed, oriented, and scaled in a vertex shader (velField.vert.glsl).
- **Boundaries**
  - Users can add boundaries to the fluid simulation grid by pressing the left mouse button and the left shift at the same time.
  - Boundaries can be removed by pressing the right mouse button and left shift at the same time.
  - Boundaries can be colored through an ImGUI color picker, leading for some artistic possibilities in the scenes.
- **Custom Scenes**
  - We have created three custom scenes (and an empty one) to showcase different fluid situations.
  - Further scenes can be easily created similarly, and we encourage users to do so.
  - For inspiration, look at crosswindscene.cpp, whirlwindscene.cpp, and waterfountainscene.cpp.

---

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

## **To-Do**
 - 3D.
---

## **Notes**
- **Memory Management**:
  - `std::vector<float>` is used instead of raw pointers (`float*`) to avoid manual memory management.
- **Main Loop Integration**:
  - Call `FluidSimulator::Tick()` from the main update loop.
- **Time Management**:
  - `FluidSimulator` can internally use a clock to determine the time delta (`dt`) between `Tick()` calls.
- Sorry I didn't get as far as I hoped, but good luck and thanks y'all!
