#version 460 core

in vec3 v_POSITION;

out vec4 COLOR;

void main()
{
	COLOR = vec4(v_POSITION, 1.0);
}
