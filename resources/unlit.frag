#version 450

layout(location = 0) in vec2 uv;

layout(location = 0) out vec4 outColor;

float hash21(vec2 p)
{
    p = fract(p * vec2(123.234, 234.34));
    p += dot(p, p + 213.42);
    return fract(p.x * p.y);
}

vec3 cairoTiling(vec2 uv, float k)
{
    vec2 id = floor(uv);
    float check = mod(id.x + id.y, 2.);

    uv = fract(uv) - 0.5;
    vec2 p = abs(uv);

    if (check == 1.)
        p = p.yx;

    // line angle
    float a = k * 3.1415;
    vec2 n = vec2(sin(a), cos(a));
    float d = dot(p - 0.5, n);
    
    if (d * (check - .5) < 0.)
        id.x += sign(uv.x) * .5;
    else
        id.y += sign(uv.y) * .5;

    d = min(d, p.x);
    d = max(d, -p.y);
    d = abs(d);
    d = min(d, dot(p - 0.5, vec2(n.y, -n.x)));

    return vec3(id, d);
}

vec3 palette(float t, vec3 a, vec3 b, vec3 c, vec3 d)
{
    return a + b * cos(6.28318 * (c * t + d));
}

void main()
{
    vec3 col = vec3(0);

    vec2 boxuv = uv;
    boxuv.x *= 16.0 / 9.0;
    boxuv *= 4.1;

    vec3 cairo = cairoTiling(boxuv, 0.65);
    col += cairo.z;

    float r = hash21(cairo.xy);
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.0, 0.1, 0.2);
    col += palette(r, a, b, c, d) * 0.8;

    col += smoothstep(0.01, 0.0, cairo.z - 0.0025);

    //if (max(p.x, p.y) > .49)
    //    col.r += 0.8;

    outColor = vec4(col, 1.0);
}
