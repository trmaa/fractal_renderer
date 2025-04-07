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
    vec3 z = point;
    float dr = 1.0;
    float scale = 2.0;

    float minRadius = 0.5;
    float fixedRadius = 1.0;

    float minRadius2 = minRadius * minRadius;
    float fixedRadius2 = fixedRadius * fixedRadius;

    for (int i = 0; i < 12; i++) {
        z = clamp(z, -1.0, 1.0) * 2.0 - z;

        float r2 = dot(z, z);
        float r = length(z);

        if (r2 < minRadius2) {
            float scaleFactor = (fixedRadius2 / minRadius2);
            z *= scaleFactor;
            dr *= scaleFactor;
        } else if (r2 < fixedRadius2) {
            float scaleFactor = (fixedRadius2 / r2);
            z *= scaleFactor;
            dr *= scaleFactor;
        }

        z = z * scale + point;
        dr = dr * abs(scale) + 1.0;
    }

    return length(z) / abs(dr);
}

vec3 fractal_normal(Fractal fractal, vec3 p) {
    float eps = 0.01;
    return normalize(vec3(
        fractal_distance(fractal, p + vec3(eps, 0.0, 0.0)) - fractal_distance(fractal, p - vec3(eps, 0.0, 0.0)),
        fractal_distance(fractal, p + vec3(0.0, eps, 0.0)) - fractal_distance(fractal, p - vec3(0.0, eps, 0.0)),
        fractal_distance(fractal, p + vec3(0.0, 0.0, eps)) - fractal_distance(fractal, p - vec3(0.0, 0.0, eps))
    ));
}

vec3 sun_dir = normalize(vec3(1.0, 1.0, 1.0));
float sun_brightness = 0.2;

Fractal fractal = Fractal(vec3(0.0, 0.0, 0.0), 1.0);


void main() {
    vec2 uv = (gl_FragCoord.xy / screen_size) * 2.0 - 1.0;
    uv.x = -uv.x * screen_size.x / screen_size.y;

    float minimum_distance = 0.0001;

    vec3 ray_idle_dir = normalize(vec3(uv.xy, 1));
    vec3 ray_dir = angle2_to_vector3_matrix(cam_ang) * ray_idle_dir;

    vec3 color = vec3(0);
    vec3 glow = vec3(0);

    float dist = 1.0;
    vec3 starting_point = cam_pos;
    int steps = 100;

    for (int i = 0; i < steps; i++) {
        dist = fractal_distance(fractal, starting_point);
        
        float glow_strength = 0.02;
        float bloom_factor = exp(-dist * 20.0);
        glow += vec3(0, 0, 0.2) * bloom_factor * glow_strength;

        starting_point += ray_dir * dist;

        if (dist <= minimum_distance) {
            vec3 normal = fractal_normal(fractal, starting_point);
            color = vec3(1.0) * float(i) / float(steps);
            color = (1.0 - color);
            //color *= normal;
            color *= vec3(length(color), 0.0, 1.0);
            color *= clamp(dot(normal, sun_dir), sun_brightness, 1.0);
            break;
        }
    }

    color += glow/3;
    color = clamp(color, 0.0, 1.0);

    gl_FragColor = vec4(color, 1.0);
}
