#version 460 core

layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec2 TEXCOORDS;

out vec3 v_POSITION;
out vec2 v_TEXCOORDS;

uniform mat4 u_view_proj_mat;

void main()
{
	gl_Position = u_view_proj_mat * vec4(POSITION, 1.0);
	v_POSITION = POSITION;
	v_TEXCOORDS = TEXCOORDS;
}
