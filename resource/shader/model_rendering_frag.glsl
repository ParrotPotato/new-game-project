#version 330 core 

in vec2 tex_coord;

out vec4 out_color;

uniform sampler2D colormap;

void main(){
    // out_color = vec4(0.0, 1.0, 0.0, 1.0);
    out_color = texture(colormap, tex_coord);
}
