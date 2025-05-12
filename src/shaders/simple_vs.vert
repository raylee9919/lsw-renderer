#version 450

layout(binding = 0, row_major) uniform Uniform_Buffer_Object {
    mat4 ortho;
} ubo;

layout(location = 0) in vec2 v_position;
layout(location = 1) in vec4 v_color;
layout(location = 2) in vec2 v_uv;

layout(location = 1) out vec4 f_color;
layout(location = 2) out vec2 f_uv;

void main() {
    gl_Position = ubo.ortho * vec4(v_position, 0.0, 1.0);
    f_color = v_color;
    f_uv = v_uv;
}
