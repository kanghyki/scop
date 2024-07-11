#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec3 center;

flat out int tri_id;
out vec2 texCoord;

void main() {
    texCoord = aTexCoord;
    tri_id = gl_VertexID / 3;
    gl_Position = proj * view * model * vec4(aPos - center, 1.0);
}
