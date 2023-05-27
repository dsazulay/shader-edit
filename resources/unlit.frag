#version 450

layout(location = 0) in vec2 uv;

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform PushData {
    float Time;
};

float hash21(vec2 p)
{
    p = fract(p * vec2(123.234, 234.34));
    p += dot(p, p + 213.42);
    return fract(p.x * p.y);
}

void main()
{
    outColor = vec4(cos(Time), 0, 1, 1);
}
