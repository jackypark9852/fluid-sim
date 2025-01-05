#version 330

out vec4 out_Color; // Output color of the fragment shader

in vec4 fs_vel;

void main()
{
    vec4 shortCol = vec4(0, 0.8, 0.4, 1);
    vec4 longCol = vec4(1, 0.1, 0.1, 1); 

    out_Color = mix(shortCol, longCol, fs_vel.a * 50.f);  
}
