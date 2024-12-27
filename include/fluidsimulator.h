#pragma once

// Macro for accessing a 1D array with 2D-like syntax. This maps 2D indices (i, j) 
// to a 1D index in a flattened array. The grid includes a boundary, 
// so its actual dimensions are (N+2) x (N+2).
#define IX(i, j) ((i) + (N + 2) * (j))


#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum class BoundaryType {
    NONE = 0,    // For scalar fields like density (no special reflection)
    HORIZONTAL,  // For horizontal velocity (u)
    VERTICAL     // For vertical velocity (v)
};

static void SWAP(std::vector<float>& x0, std::vector<float>& x) {
    std::vector<float>& swap = x0;
    x0 = x;
    x = swap;
}

class FluidSimulator {
private:    
    unsigned int N;          // The width of the inner grid (non-boundary cells) excluding the boundary.
                             // The total grid dimensions are (N+2) x (N+2) to account for boundaries.

    unsigned int elemCount;  // Total number of elements in the grid, including boundaries.
                             // This equals (N+2) * (N+2).

    // Texture representing normalized density field as a greyscale image
    GLuint densityTextureHandle; 

    // Horizontal velocity components of the fluid at the current time step
    std::vector<float> u;

    // Horizontal velocity components of the fluid at the previous time step
    std::vector<float> u_prev;

    // Vertical velocity components of the fluid at the current time step
    std::vector<float> v;

    // Vertical velocity components of the fluid at the previous time step
    std::vector<float> v_prev;

    // Density values of the fluid at the current time step
    std::vector<float> dens;

    // Density values of the fluid at the previous time step
    std::vector<float> dens_prev;

    float viscosity;

    float diffusion;
    /// <summary>
    /// Adds a source term to the given grid by incrementing its values.
    /// </summary>
    /// <param name="N">The size of the grid (excluding boundaries).</param>
    /// <param name="x">The grid to which the source values will be added.</param>
    /// <param name="s">The source grid containing the values to add.</param>
    /// <param name="dT">The time step for scaling the source contribution.</param>
    void AddSource(int N, std::vector<float>& x, const std::vector<float>& s, float dT);

    /// <summary>
    /// Diffuses the scalar field over the grid, spreading values according to the diffusion coefficient.
    /// </summary>
    /// <param name="N">The size of the grid (excluding boundaries).</param>
    /// <param name="b">The type of boundary condition to apply (e.g., none, horizontal, vertical).</param>
    /// <param name="x">The grid containing the diffused values after the function completes.</param>
    /// <param name="x0">The grid containing the initial values before diffusion.</param>
    /// <param name="diff">The diffusion coefficient controlling the rate of diffusion.</param>
    /// <param name="dt">The time step over which diffusion occurs.</param>
    void Diffuse(int N, BoundaryType b, std::vector<float>& x, const std::vector<float>& x0, float diff, float dt);

    /// <summary>
    /// Moves scalar values (e.g., density) through the grid based on the velocity field.
    /// </summary>
    /// <param name="N">The size of the grid (excluding boundaries).</param>
    /// <param name="b">The type of boundary condition to apply (e.g., none, horizontal, vertical).</param>
    /// <param name="d">The grid containing the advected values after the function completes.</param>
    /// <param name="d0">The grid containing the initial values before advection.</param>
    /// <param name="u">The horizontal velocity field.</param>
    /// <param name="v">The vertical velocity field.</param>
    /// <param name="dt">The time step over which advection occurs.</param>
    void Advect(int N, BoundaryType b, std::vector<float>& d, const std::vector<float>& d0, const std::vector<float>& u,
        const std::vector<float>& v, float dt);

    /// <summary>
    /// Performs a full simulation step for the density field, including diffusion and advection.
    /// </summary>
    /// <param name="N">The size of the grid (excluding boundaries).</param>
    /// <param name="x">The density field after the simulation step completes.</param>
    /// <param name="x0">The density field from the previous time step.</param>
    /// <param name="u">The horizontal velocity field.</param>
    /// <param name="v">The vertical velocity field.</param>
    /// <param name="diff">The diffusion coefficient controlling the rate of diffusion.</param>
    /// <param name="dt">The time step for the simulation step.</param>
    void DensStep(int N, std::vector<float>& x, std::vector<float>& x0, const std::vector<float>& u,
        const std::vector<float>& v, float diff, float dt);

    /// <summary>
    /// Performs a full simulation step for velocity field
    /// </summary>
    /// <param name="N">The size of the grid (excluding boundaries).</param>
    /// <param name="u">The horizontal velocity field after the simulation step completes.</param>
    /// <param name="u0">The horizontal velocity field from the previous time step.</param>
    /// <param name="v">The vertical velocity field after the simulation step completes.</param>
    /// <param name="v0">The vertical velocity field from the previous time step.</param>
    /// <param name="visc">The viscosity coefficient controlling the rate of diffusion.</param>
    /// <param name="dt">The time step for the simulation step.</param>
    void VelStep(int N, std::vector<float>& u, std::vector<float>& v, std::vector<float>& u0, std::vector<float>& v0,
        float visc, float dt);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="N"></param>
    /// <param name="u"></param>
    /// <param name="v"></param>
    /// <param name="p"></param>
    /// <param name="div"></param>
    /// todo: documentation here 
    void Project(int N, std::vector<float>& u, std::vector<float>& v, std::vector<float>& p, std::vector<float>& div);

    /// <summary>
    /// Applies boundary conditions to a scalar field on the simulation grid.
    /// </summary>
    /// <param name="N">The size of the inner grid (excluding boundary cells).</param>
    /// <param name="b"> The type of boundary condition to apply </param>
    /// <param name="x">The grid containing the scalar field to which boundary conditions are applied.</param>
    void SetBoundaryConditions(int N, BoundaryType b, std::vector<float>& x);

    /// <summary>
    /// Updates the OpenGL texture with the current density field values.
    /// </summary>
    void UpdateDensityTexture();

public: 
    /// <summary>
    /// Constructs a FluidSimulator object with a grid size of (N+2) x (N+2), 
    /// including boundary cells.
    /// </summary>
    /// <param name="N">The width (and height) of the inner grid, excluding boundary cells. Defaults to 100.</param>
    FluidSimulator(unsigned int N = 100);


    /// <summary>
    /// Gets the current horizontal velocity components of the fluid.
    /// </summary>
    /// <returns>A constant reference to the vector representing the horizontal velocities (u).</returns>
    const std::vector<float>& GetU() const;

    /// <summary>
    /// Gets the current vertical velocity components of the fluid.
    /// </summary>
    /// <returns>A constant reference to the vector representing the vertical velocities (v).</returns>
    const std::vector<float>& GetV() const;

    /// <summary>
    /// Gets the current density values of the fluid.
    /// </summary>
    /// <returns>A constant reference to the vector representing the fluid densities.</returns>
    const std::vector<float>& GetDens() const;

    /// <summary>
    /// Advances the simulation by one time step, performing all necessary updates to the fluid's state.
    /// </summary>
    void Tick();

    /// <summary>
    /// Retrieves the OpenGL texture handle for the density field.
    /// </summary>
    /// <returns>The OpenGL texture handle associated with the density field.</returns>
    GLuint GetDensityTextureHandle() const;
};
            