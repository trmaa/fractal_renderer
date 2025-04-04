#version 330 core

uniform vec2 screen_size;
uniform vec3 cam_pos;
uniform vec2 cam_ang;

#include "tools.glsl"
#include "scene.glsl"

void main() {
    vec2 uv = (gl_FragCoord.xy / screen_size) * 2.0 - 1.0;
    uv.x = -uv.x * screen_size.x / screen_size.y;

    vec3 ray_idle_dir = normalize(vec3(uv.xy, 5.0));
    vec3 ray_dir = angle2_to_vector3_matrix(cam_ang) * ray_idle_dir;

    vec3 color = vec3(0);

    //color will depend on if it hits scene objects (aka: 3d fractals) or not
    color = normalize(ray_dir);

    gl_FragColor = vec4(color, 1.0);
}
