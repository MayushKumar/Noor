#version 450 core

layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec3 NORMAL;
layout(location = 2) in vec2 TEXCOORDS;

out vec3 v_normal;
out vec2 v_texcoords;

uniform mat4 u_view_proj_mat;

void main()
{
	gl_Position = u_view_proj_mat * vec4(POSITION, 1.0);

	v_normal = NORMAL;
	v_texcoords = TEXCOORDS;
}
