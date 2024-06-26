/*

  This assumes a Lambertian BRDF. Therefore, to use in the PBR shader,
  just multiply the value of the cubemap by the color. The division
  by PI has been taken care of here itself.

*/

#version 450 core

in vec3 v_dir;
in vec3 v_right;

out vec4 COLOR;

uniform samplerCube u_envmap;

#define PI 3.14159265358979

const int N = 2500;

float VdC(uint bits);
vec2 Hammersley2D(uint i, uint N);
vec3 sample_cos(float u, float v);
vec3 sample_uniform(float u, float v);

void main()
{
	vec3 dir = normalize(v_dir);
	vec3 bitangent = normalize(cross(dir, v_right));
	vec3 tangent = normalize(cross(dir, bitangent));
	vec3 val = vec3(0.0);
	int i = 0;
	while(i < N)
	{
		vec2 hamm_vec = Hammersley2D(i, N);
		vec3 tan_space_vec = sample_uniform(hamm_vec.x, hamm_vec.y);
		vec3 sampling_dir = tan_space_vec.z * dir + tan_space_vec.x * tangent + tan_space_vec.y * bitangent;
		val += clamp(textureLod(u_envmap, sampling_dir, 4).rgb, vec3(0.0), vec3(20.0)) * dot(sampling_dir, dir);
		i++;
	}

	COLOR = vec4(val / N, 1.0);
}

float VdC(uint bits)
{
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 Hammersley2D(uint i, uint N)
{
	return vec2(float(i)/float(N), VdC(i));
}

vec3 sample_uniform(float u, float v) {
	float phi = v * 2.0 * PI;
	float cosTheta = 1.0 - u;
	float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
	return vec3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
}
    
vec3 sample_cos(float u, float v) {
	float phi = v * 2.0 * PI;
	float cosTheta = sqrt(1.0 - u);
	float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
	return vec3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
}
