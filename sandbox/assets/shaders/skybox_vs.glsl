#version 450 core

layout(location = 0)  in vec3 POSITION;

uniform mat4 u_view_proj_mat;

out vec3 v_PosDir;

void main()
{
	// gl_Position = vec4(mat3(u_view_proj_mat) * POSITION, 1.0);
	gl_Position = u_view_proj_mat * vec4(POSITION, 1.0);
	v_PosDir = POSITION;
}
