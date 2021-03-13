#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 color;

out vec4 vertex_color;

void main()
{
    gl_Position = position;
    vertex_color = vec4(1.0, color.y, color.x, 0.0);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 vertex_color;

void main()
{
    color = vertex_color;
};