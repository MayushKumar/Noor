#version 460 core

layout(location = 0) in vec3 POSITION;

out vec3 v_POSITION;

void main()
{
	gl_Position = vec4(POSITION, 1.0);
	v_POSITION = POSITION;
}
