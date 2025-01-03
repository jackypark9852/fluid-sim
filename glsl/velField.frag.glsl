#version 330

out vec4 out_Color; // Output color of the fragment shader

uniform sampler2D u_Texture; // Sampler for the texture

void main()
{
    // Sample the texture at the given coordinates and output the color
    // out_Color = texture(u_Texture, fs_UV);
    out_Color = vec4(1, 0, 0, 1);
}
