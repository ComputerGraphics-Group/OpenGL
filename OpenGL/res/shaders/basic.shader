#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

//out vec4 vertex_color;

void main()
{
    gl_Position = position;
//    vertex_color = position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

//in vec4 vertex_color;

void main()
{
    color = vec4( 1, 1, 0, 1 );
};