#version 450 core

layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec2 TEXCOORDS;

out vec2 v_TEXCOORDS;

void main()
{
	gl_Position = vec4(2 * POSITION, 1.0);
	v_TEXCOORDS = TEXCOORDS;
}
