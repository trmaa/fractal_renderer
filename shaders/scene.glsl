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
    float Power = 8.0 * abs(sin(i_time)) + 5.0;

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
