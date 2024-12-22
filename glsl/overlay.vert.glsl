#version 150
// ^ Change this to version 130 if you have compatibility issues

// Refer to the lambert shader files for useful comments
in vec4 vs_Pos; // Vertex position
in vec2 vs_UV; // Vertex texture coordinate

out vec2 fs_UV; // Texture coordinate passed down to the fragment shader

void main()
{
    fs_UV = vs_UV;
    gl_Position = vs_Pos;
}
