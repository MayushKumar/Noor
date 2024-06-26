#version 450 core

layout(location = 0)  in vec3 POSITION;

uniform mat4 u_orient_mat;

out vec3 v_sampling_vec;

void main()
{
	gl_Position = vec4(2.0 * POSITION, 1.0);
	v_sampling_vec = (u_orient_mat * vec4(POSITION, 1.0)).xyz;
}
