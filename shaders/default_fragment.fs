#version 330 core

in vec3 Normal;

out vec4 FragCol;

void main()
{
    vec3 lightDir = vec3(1.0, 2.0, 0.5);
    float mag = dot(Normal, normalize(lightDir));

    FragCol = vec4(abs(vec3(Normal)), 1.0);
    return;

    if(mag <= 0.0)
        FragCol = vec4(0.0, 0.0, 0.0, 1.0);
    else
        FragCol = vec4(mag, mag, mag, 1.0);
}