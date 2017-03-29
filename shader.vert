#version 120

attribute vec2 in_Vertex;

varying vec2 pos;

void main()
{

    gl_Position = vec4(in_Vertex, 0, 1);

    pos = in_Vertex;

}
