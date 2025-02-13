#version 330

in vec3 vertex_color;
in float noise_value;

out vec4 frag_color;

void main() {
    vec3 color1 = vec3(0.5, 0.6, 0.8);
    vec3 color2 = vec3(0.6, 0.7, 0.5); 

    vec3 final_color = mix(color1, color2, noise_value);

    frag_color = vec4(final_color, 1.0);
}