#version 330

in vec4 vs_Pos; // Vertex position

uniform sampler2D u_Texture; // Sampler for the texture

flat out int id; // the instance ID of this arrow
out vec4 fs_vel;

void main()
{
    int id = gl_InstanceID;
    vec2 pos = vec2(id % 100, id / 100);                // find the grid position of this arrow by ID
    mat4 trans = mat4(1);                               // initialize transform matrix as identity
    vec4 vel = texture(u_Texture, pos * 0.01);          // velocity equals the texture pos at adjusted pos value
    // converting angles back to [-1, 1] from [0, 1]
    vel.x = (vel.x * 2.f) - 1;
    vel.y = (vel.y * 2.f) - 1;
    vel.z = (vel.z * 2.f) - 1;
    fs_vel = vel;                                       // sending vel to fragment shader
    
    float angle = atan(vel.y, vel.x);                   // getting angle from velocity y and x val
    if (angle < 0) {
        angle += 2 * 3.14159265;                        // atan is [-pi, pi], convert to [0, 2pi]
    }
    angle = 3.14159265 * 0.5 - angle;  // arrow points up by default, so we rotate right - angle
    // Z rotation matrix by angle
    mat4 rotZ = mat4(
        cos(angle), -sin(angle), 0.0, 0.0,
        sin(angle),  cos(angle), 0.0, 0.0,
        0.0,         0.0,        1.0, 0.0,
        0.0,         0.0,        0.0, 1.0
    );
    // scaling arrow size by vector length
    trans[0] *= vel.a * 75.f;
    trans[1] *= vel.a * 75.f;

    trans = rotZ * trans;

    trans[3].xy = vec2(-0.995, -0.995) + pos * 0.02;

    gl_Position = trans * vs_Pos;
}
