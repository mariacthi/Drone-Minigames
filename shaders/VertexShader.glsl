#version 330

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;
layout(location = 2) in vec3 in_normal;

out vec3 vertex_color;
out float noise_value;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);

    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

void main() {
    vec4 world_position = Model * vec4(in_position, 1.0);
    vec2 noise_coords = world_position.xz * 0.1;
    float height = noise(noise_coords) * 2.0; 

    world_position.y += height;
    noise_value = height;

    gl_Position = Projection * View * world_position;
    vertex_color = in_color;
}