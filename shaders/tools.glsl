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
