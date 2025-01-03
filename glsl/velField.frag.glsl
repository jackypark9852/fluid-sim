#version 330

out vec4 out_Color; // Output color of the fragment shader

uniform sampler2D u_Texture; // Sampler for the texture

void main()
{
    // Sample the texture at the given coordinates and output the color
    // out_Color = texture(u_Texture, fs_UV);
    // vel at 25, 50
    vec4 vel = texture(u_Texture, vec2(0.25, 0.5));
    out_Color = vec4(vel.r, vel.g, vel.b, 1);
}
