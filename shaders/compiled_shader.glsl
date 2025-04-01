#version 330 core

uniform vec2 resolution;

void main() {
	vec2 uv = gl_FragCoord.xy / resolution.x;

	gl_FragColor = vec4(uv, 0, 1.0);
}
