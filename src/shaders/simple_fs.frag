#version 450

layout(location = 1) in vec4 f_color;
layout(location = 2) in vec2 f_uv;

layout(location = 0) out vec4 result;

layout(binding = 1) uniform sampler2D texture_sampler;

void main() {
    result = texture(texture_sampler, f_uv) * f_color;
}
