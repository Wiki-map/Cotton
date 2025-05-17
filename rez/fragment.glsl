#version 330 core

in vec4 v_color;
in vec2 v_uv;
in float v_texindex;

out vec4 FragColor;

uniform sampler2D u_tex[8];

void main() {
    switch(int(v_texindex)) {
        case 0: FragColor = v_color * texture(u_tex[0],v_uv); break;
        case 1: FragColor = v_color * texture(u_tex[1],v_uv); break;
        case 2: FragColor = v_color * texture(u_tex[2],v_uv); break;
        case 3: FragColor = v_color * texture(u_tex[3],v_uv); break;
        case 4: FragColor = v_color * texture(u_tex[4],v_uv); break;
        case 5: FragColor = v_color * texture(u_tex[5],v_uv); break;
        case 6: FragColor = v_color * texture(u_tex[6],v_uv); break;
        case 7: FragColor = v_color * texture(u_tex[7],v_uv); break;
        default: discard;
    }
}
