#version 460 core

layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec3 NORMAL;
layout(location = 2) in vec4 TANGENT;
layout(location = 3) in vec2 TEXCOORDS;
// layout(location = 4) in vec3 BITANGENT;

out vec3 v_normal;
out vec4 v_tangent;
out vec2 v_texcoords;
out vec3 v_view_pos;
out vec3 v_light_pos;
out vec3 v_pos;

uniform vec3 u_view_pos;
uniform mat4 u_view_proj_mat;
uniform mat4 u_model_mat;
uniform mat4 u_normal_mat;


struct PointLight
{
	vec3 pos;
	float intensity;
};
uniform PointLight light;
out PointLight v_light;

void main()
{
	vec4 position = u_model_mat * vec4(POSITION, 1.0);
	gl_Position = u_view_proj_mat * position;

	v_normal = (u_normal_mat * vec4(NORMAL, 0.0)).xyz;
	v_tangent = vec4((u_normal_mat * vec4(TANGENT.xyz, 0.0)).xyz, TANGENT.w);
	v_view_pos = u_view_pos;
	v_light = light;
	v_pos = position.xyz;
	v_texcoords = TEXCOORDS;
}
