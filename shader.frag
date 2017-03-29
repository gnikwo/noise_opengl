#version 120

uniform sampler2D gradient;

uniform vec2 center;
uniform float scale;
uniform int iter;

varying vec2 pos;

void main()
{

    vec2 z, c;

    c.x = (pos.x)*scale - center.x;
    c.y = (pos.y)*scale - center.y;

    int i;
    z = c;
    for(i=0; i<iter; i++) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
        float y = (z.y * z.x + z.x * z.y) + c.y;

        if((x * x + y * y) > 4.0)
            break;

        z.x = x;
        z.y = y;

    }

gl_FragColor = texture2D(gradient, vec2(i == iter ? 0.0 : float(i), 0) / 100.0);

}
