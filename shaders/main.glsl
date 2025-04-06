#version 330 core

uniform vec2 screen_size;
uniform vec3 cam_pos;
uniform vec2 cam_ang;
uniform float i_time;

#include "tools.glsl"
#include "scene.glsl"

void main() {
    vec2 uv = (gl_FragCoord.xy / screen_size) * 2.0 - 1.0;
    uv.x = -uv.x * screen_size.x / screen_size.y;

    float minimum_distance = 0.0001;

    vec3 ray_idle_dir = normalize(vec3(uv.xy, 2.5));
    vec3 ray_dir = angle2_to_vector3_matrix(cam_ang) * ray_idle_dir;

    vec3 color = vec3(0);
    vec3 glow = vec3(0);

    float dist = 1.0;
    vec3 starting_point = cam_pos;
    int steps = 50;

    for (int i = 0; i < steps; i++) {
        dist = fractal_distance(fractal, starting_point);
        
        float glow_strength = 0.1;
        float bloom_factor = exp(-dist * 15.0);
        glow += vec3(0.8, 0.5, 0.1) * bloom_factor * glow_strength;

        starting_point += ray_dir * dist;

        if (dist <= minimum_distance) {
            vec3 normal = fractal_normal(fractal, starting_point);
            color = vec3(1.0) * float(i) / float(steps);
            color = (1.0 - color);
            //color *= normal;
            color *= vec3(length(color), 0.0, 1.0);
            color *= clamp(dot(normal, -sun_dir), sun_brightness, 1.0);
            break;
        }
    }

    color += glow/3;
    color = clamp(color, 0.0, 1.0);

    gl_FragColor = vec4(color, 1.0);
}
