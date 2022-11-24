#version 460 core

in vec3 v_POSITION;
in vec2 v_TEXCOORDS;

uniform sampler2D u_texture;

out vec4 COLOR;

void main()
{
	COLOR = vec4(v_TEXCOORDS, 0.0, 1.0);
	COLOR = texture(u_texture, v_TEXCOORDS);
}
