#version 330 core

uniform sampler2D tex;
uniform bool is_tex;

out vec4 fragColor;

flat in int tri_id;
in vec2 texCoord;

void main() {
    if (is_tex) {
        vec4 tex_color = texture(tex, texCoord);
        if (tex_color.a < 0.1) {
            discard;
        }
        fragColor = tex_color;
    }
    else {
        float c = float((tri_id * 101) % 256) / 255.0;
        fragColor = vec4(c, c, c, 1.0);
    }
}
