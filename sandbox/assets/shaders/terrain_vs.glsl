#version 450 core

layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec3 NORMAL;

uniform mat4 u_view_proj_mat;

out vec3 v_normal;
out vec3 v_view_dir;

uniform vec3 u_view_pos;

void main()
{
	vec4 POS = u_view_proj_mat * vec4(POSITION, 1.0);
	gl_Position = POS;
	v_view_dir = u_view_pos - POSITION.xyz;
	v_normal = NORMAL;
}
