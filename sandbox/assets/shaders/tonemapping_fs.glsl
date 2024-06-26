#version 450 core

in vec2 v_TEXCOORDS;

uniform sampler2D u_texture;

out vec4 COLOR;

void main()
{
	COLOR = texture(u_texture, v_TEXCOORDS);
	COLOR = vec4(pow(COLOR.xyz, vec3(1.0 / 2.2)), 1.0);
}
