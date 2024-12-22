#version 150
// ^ Change this to version 130 if you have compatibility issues

// Refer to the lambert shader files for useful comments
uniform sampler2D u_Texture;
uniform int u_BlockType; // Block type: 0 = none, 1 = water, 2 = lava
uniform vec2 u_Dimensions;

uniform float u_Time;

in vec2 fs_UV;

out vec4 out_Col;

// p = query point
// r = thickness
float sdCross(in vec2 p, in vec2 b, float r){
    p = abs(p);
    p = (p.y > p.x) ? p.yx : p.xy;
    vec2 q = p - b;
    float k = max(q.y, q.x);
    vec2 w = (k > 0.0) ? q : vec2(b.y-p.x, -k);
    return sign(k) * length(max(w, 0.5)) + r;
}

// NOISE GENERATION
// random func for placing our worley points and bubbles
vec2 rPoint (vec2 p) {
    return fract(sin(vec2(dot(p, vec2(101.01, 17.22)), dot(p, vec2(429.123, 188.88)))) * 3141.5927);
}

// a funkier sin function
float sinner(float x) {
    return (sin(x * 2.f * 3.14159265) + 1.f) / 2.f;
}

// this function is a mix from the lecture slides and the shadertoy
// voronoi function that I modified for my uses
// added a time variable to change noise values with time
// grid size is called outside of the func cause I like it that way
float WorleyNoise(vec2 uv, float time, float gridSize, float offset) {
    uv *= gridSize;
    vec2 uvInt = floor(uv);
    // this extra -0.5f helps us with some edges between the worley grids
    vec2 uvFract = fract(uv) - 0.5f;
    float minDist = 1.0; // Minimum distance initialized to max.
    for(int y = -1; y <= 1; ++y) {
        for(int x = -1; x <= 1; ++x) {
            vec2 neighbor = vec2(float(x), float(y)); // Direction in which neighbor cell lies
            // Get the Voronoi centerpoint for the neighboring cell with offset
            vec2 point = neighbor + sin(rPoint(uvInt + neighbor + offset) * (time + 10.f)) * 0.5;
            // Distance between fragment coord and neighbor’s Voronoi point
            float dist = distance(point, uvFract);
            minDist = min(dist, minDist);
        }
    }
    return minDist;
}

void main()
{
    // Sample the texture color
    vec4 color = texture(u_Texture, fs_UV);

    bool isWater = u_BlockType == 1;
    bool isLava = u_BlockType == 2;

    // creating an overlay for being in water or lava
    if (isWater || isLava) {
        // Generating basic distortion overlay
        // These determine the water-wave distortion amounts
        float timeScale = u_Time * 0.5;
        float amp = .05f;
        float turb = 0.3f;
        // Calculating the distortion at UV level
        vec2 uvDistort = fs_UV;
        uvDistort += sinner(uvDistort.x * turb + timeScale) * amp;
        uvDistort -= sinner(uvDistort.y * turb + timeScale) * amp;
        // the info from the pre-process image with the distortion applied
        vec3 pixel = texture(u_Texture, uvDistort).rgb;

        // add 2 worleys
        float extraDistort = abs(uvDistort.x) / 3.f;
        float worley = WorleyNoise(uvDistort, timeScale * 2, 5.f - extraDistort, 0.5) * 0.6;
        worley += WorleyNoise(uvDistort, timeScale * 4, 8.f - extraDistort, 0.f) * 0.3;

        if (isWater) {
            //foreground blue water tint
            pixel *= vec3(0.7f, 0.7f, 0.9f);
            vec3 col = worley * vec3(0.3, 0.3, 0.9);
            //background is the image
            col += clamp((1 - worley) * pixel, vec3(0, 0, 0), vec3(0.9, 0.9, 1)) * 0.8;
            color = vec4(col.xyz, color.a);
        } else {
            //foreground red lava tint
            pixel *= vec3(1.f, 0.3f, 0.3f);
            pixel += vec3(0.3, 0.f, 0.f);
            vec3 col = worley * vec3(0.9, 0.3, 0.3);
            //background is the image
            col += clamp((1 - worley) * pixel, vec3(0, 0, 0), vec3(1, 0.9, 0.9)) * 0.8;
            color = vec4(col.xyz, color.a);
        }
    }

    // drawing plus in the center of the screen with opposite color
    // Finds center of the screen
    vec2 center = u_Dimensions / 2;
    vec2 uv = fs_UV * 2 - vec2(1.);
    float crossSDF = sdCross(uv, vec2(.008f,.002f), .01f);
    // Checks if screen point is within range and inverts it if it is
    if (crossSDF <= 0){
        color.rgb = vec3(1, 1, 1) - color.rgb;
        color.a = 1.f;
    }

    out_Col = color;
}
