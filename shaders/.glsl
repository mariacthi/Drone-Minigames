#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 vertex_color;

float random(vec2 st) {
    return fract(sin(dot(st, vec2(12.9898, 78.233))) * 43758.5453123);
}

float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) +
           (c - a) * u.y * (1.0 - u.x) +
           (d - b) * u.x * u.y;
}

void main() {
    vec3 pos = in_position;

    // Modificarea în?l?imii folosind noise
    float frequency = 0.1;
    float amplitude = 1.0;
    pos.y += noise(pos.xz * frequency) * amplitude;

    // Calcularea culorii pe baza în?l?imii
    vec3 lowColor = vec3(0.2, 0.6, 0.2); // Verde
    vec3 highColor = vec3(1.0, 1.0, 1.0); // Alb
    vertex_color = mix(lowColor, highColor, pos.y / amplitude);

    gl_Position = Projection * View * Model * vec4(pos, 1.0);
}