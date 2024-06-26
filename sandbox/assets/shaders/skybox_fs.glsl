#version 450 core

in vec3 v_PosDir;

uniform samplerCube u_env_map;

out vec4 COLOR;

void main()
{
	COLOR = texture(u_env_map, normalize(v_PosDir));
	COLOR = textureLod(u_env_map, normalize(v_PosDir), 0.0);
}
