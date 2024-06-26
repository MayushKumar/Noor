#version 450 core

layout(location = 0)  in vec3 POSITION;

uniform mat4 u_orient_mat;

out vec3 v_dir;
out vec3 v_right;

void main()
{
	gl_Position = vec4(2.0 * POSITION, 1.0);
	vec3 pos_dir = POSITION;

	v_dir = normalize((u_orient_mat * vec4(pos_dir, 1.0)).xyz);
	v_right = mat3(u_orient_mat) * vec3(1.0, 0.0, 0.0);
}
