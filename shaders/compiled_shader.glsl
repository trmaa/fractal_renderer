#version 330 core

uniform vec2 screen_size;
uniform vec3 cam_pos;
uniform vec2 cam_ang;
uniform float i_time;

mat3 angle2_to_vector3_matrix(vec2 angle2) {
    float cos_pitch = cos(angle2.y);
    float sin_pitch = sin(angle2.y);
    float cos_yaw = cos(-angle2.x);
    float sin_yaw = sin(-angle2.x);

    mat3 R_x = mat3(
        1, 0, 0,
        0, cos_pitch, -sin_pitch,
        0, sin_pitch, cos_pitch
    );

    mat3 R_y = mat3(
        cos_yaw, 0, sin_yaw,
        0, 1, 0,
        -sin_yaw, 0, cos_yaw
    );

    mat3 R = R_y * R_x;

    return R;
}

float random(vec3 seed) {
    return fract(sin(dot(seed /*+ vec3(iTime)*/, vec3(12.9898, 78.233, 45.164))) * 43758.5453);
}

struct Fractal {
    vec3 center;
    float radius;
};

/*float fractal_distance(Fractal fractal, vec3 point) {
    return length(point - fractal.center) - fractal.radius;
}*/

float fractal_distance(Fractal fractal, vec3 point) {
    vec3 tileSize = vec3(4.0);
    point = mod(point + 0.5 * tileSize, tileSize) - 0.5 * tileSize;

    vec3 z = point;
    float dr = 1.0;
    float r = 0.0;
    
    int Iterations = 6;
    float Power = 8.0 * abs(sin(i_time)) + 2.0;

    for (int i = 0; i < Iterations; i++) {
        r = length(z);
        if (r > 2.0) break;

        float theta = acos(z.z / r);
        float phi = atan(z.y, z.x);
        float zr = pow(r, Power - 1.0);

        dr = zr * Power * dr + 1.0;

        zr *= r;
        theta *= Power;
        phi *= Power;

        z = zr * vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
        z += point;
    }

    return 0.5 * log(r) * r / dr;
}

vec3 fractal_normal(Fractal fractal, vec3 p) {
    float eps = 0.001; // Try tweaking this if output is still zero
    return normalize(vec3(
        fractal_distance(fractal, p + vec3(eps, 0.0, 0.0)) - fractal_distance(fractal, p - vec3(eps, 0.0, 0.0)),
        fractal_distance(fractal, p + vec3(0.0, eps, 0.0)) - fractal_distance(fractal, p - vec3(0.0, eps, 0.0)),
        fractal_distance(fractal, p + vec3(0.0, 0.0, eps)) - fractal_distance(fractal, p - vec3(0.0, 0.0, eps))
    ));
}

vec3 sun_dir = normalize(vec3(1.0, 1.0, 1.0));
float sun_brightness = 0.5;

Fractal fractal = Fractal(vec3(0.0, 0.0, 0.0), 1.0);


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
