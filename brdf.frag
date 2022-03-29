#version 330 core

// This is NOT a BRDF lighting model, it only shows solid color and that's it!

in vec3 worldPosition;
in vec3 normal;
in vec3 tangent;
in vec2 uv;

out vec4 fragColor;

void main() {
	fragColor = vec4(worldPosition, 1.0);
}