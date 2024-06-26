#version 450 core

in vec3 v_sampling_vec;

out vec4 COLOR;

uniform sampler2D u_envmap;

#define PI 3.14159

void main()
{
	vec3 dir = normalize(v_sampling_vec);
	COLOR = texture(u_envmap, vec2(atan(dir.x, -dir.z) / (2 * PI) + 0.5, (asin(dir.y) / PI + 0.5)));
}
