#version 330

in vec4 vs_Pos; // Vertex position

uniform sampler2D u_Texture; // Sampler for the texture

void main()
{
    int id = gl_InstanceID + 1;
    vec2 pos = vec2(id % 100, id / 100);
    mat4 trans = mat4(1);
    vec4 vel = texture(u_Texture, pos);
    // float height = vs_Pos.y;
    float angle = cos(normalize(vel.xy).x);

    mat4 rotZ = mat4(
        cos(angle), -sin(angle), 0.0, 0.0,
        sin(angle),  cos(angle), 0.0, 0.0,
        0.0,         0.0,        1.0, 0.0,
        0.0,         0.0,        0.0, 1.0
    );

    trans = rotZ * trans;

    // trans[3].xy = pos;

    trans[3].xy = vec2(-1, -1) + pos * 0.02;

    gl_Position = trans * vs_Pos;
}
