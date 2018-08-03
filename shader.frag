#version 120

uniform int time;

varying vec2 pos;

float rand(vec2 n)
{
    return 0.5 + 0.5 * fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

void main()
{

    gl_FragColor = vec4(rand(pos.xx * pos.y * time), rand(pos.yy * pos.x * time), rand(pos.xy * time), 1.0);

}
