#version 330 core

uniform vec2 resolution;

void main() {
	vec2 uv = gl_FragCoord.xy / resolution.x;
	vec3 color = vec3(1.0, 0.0, 1.0);

	gl_FragColor = vec4(color, 1.0);
}
