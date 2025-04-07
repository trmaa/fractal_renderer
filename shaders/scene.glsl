struct Fractal {
    vec3 center;
    float radius;
};

/*float fractal_distance(Fractal fractal, vec3 point) {
    return length(point - fractal.center) - fractal.radius;
}*/

float fractal_distance(Fractal fractal, vec3 point) {
    vec3 tileSize = vec3(20.0);
    point = mod(point + 0.5 * tileSize, tileSize) - 0.5 * tileSize;

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
