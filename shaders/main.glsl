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

    float minimum_distance = 0.00001;

    vec3 ray_idle_dir = normalize(vec3(uv.xy, 2.5));
    vec3 ray_dir = angle2_to_vector3_matrix(cam_ang) * ray_idle_dir;

    vec3 color = vec3(0);//ray_dir;

    //set color
    float dist = 1;
    vec3 starting_point = cam_pos;
    int steps = 100;
    for (int i = 0; i < steps; i++) {
        dist = fractal_distance(fractal, starting_point);
        starting_point += ray_dir * dist;
        vec3 normal = normalize(starting_point - fractal.center);
        if (dist <= minimum_distance) {
            color = vec3(1) * i/steps;// * dot(normal, sun_dir);
            color = 1 - color;
            break;
        }
    }
    //color setted

    color = clamp(color, 0.0, 1.0);

    gl_FragColor = vec4(color, 1.0);
}
