#version 460 core

in vec3 v_normal;
in vec2 v_texcoords;

out vec4 COLOR;

struct Material
{
	sampler2D base_tex;
	sampler2D normal_tex;
	sampler2D metallic_roughness_tex;
};

uniform Material material;

void main()
{
	COLOR = vec4(0.2, 0.6, 0.7, 1.0);
	COLOR = vec4(v_normal, 1.0);
	COLOR = texture(material.base_tex, vec2(v_texcoords.x, 1.0 - v_texcoords.y));
}
