#version 330 core 

layout(location = 0) in vec3 position;

out vec4 fragment_color;

void main(){
    gl_Position = vec4(position, 1.0);
    fragment_color = vec4(1.0, 0.3, 1.0, 1.0);
}