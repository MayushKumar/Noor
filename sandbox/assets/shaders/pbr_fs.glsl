#version 460 core

#define PI 3.14159

in vec3 v_normal;
in vec4 v_tangent;
in vec3 v_bitangent;
in vec3 v_cross;
in vec2 v_texcoords;
in vec3 v_view_pos;

in mat3 tbn;

out vec4 COLOR;

struct Material
{
	sampler2D base_tex;
	sampler2D normal_tex;
	sampler2D metallic_roughness_tex;
};

struct PointLight
{
	vec3 pos;
	float intensity;
};

uniform samplerCube u_envmap;
uniform samplerCube u_irradmap;
uniform samplerCube u_radmap;
uniform sampler2D u_dfglut;

uniform Material material;
in PointLight v_light;
in vec3 v_pos;

const float ENV_MULTIPLIER = 2.0;

float D_GGX(float NoH, float alpha);
float V_SmithGGXCorrelated(float NoV, float NoL, float alpha);
float V_SmithGGXCorrelatedFast(float NoV, float NoL, float alpha);
vec3 F_Schlick_opt(float VoH, vec3 f0);
vec3 F_Schlick(float VoH, vec3 f0, float f90);

void main()
{
	vec3 bitangent = v_tangent.w * cross(v_normal, v_tangent.xyz);
	vec3 normal = texture(material.normal_tex, v_texcoords).rgb * vec3(2.0) - vec3(1.0);
	vec3 albedo = texture(material.base_tex, v_texcoords).rgb; // using an sRGB texture, no need for gamma correction
	vec2 metallic_roughness = texture(material.metallic_roughness_tex, v_texcoords).bg;
	float roughness = metallic_roughness.y;
	float alpha = roughness * roughness;
	float metallic = metallic_roughness.x;

	vec3 diffuse_color = (1.0 - metallic) * albedo;
	normal = normalize(mat3(v_tangent.xyz, bitangent, v_normal) * normal);

	// vec3 test_light_pos = vec3(5.0, 0.2, -1.0);
	// float test_light_power = 100.0;
	vec3 view_dir = normalize(v_view_pos - v_pos);
	// vec3 light_dir = normalize(test_light_pos - v_pos);
	// float falloff_coeff = 1.0/pow(length(test_light_pos - v_pos), 2);

	// vec3 H = normalize(light_dir + view_dir);
	vec3 R = reflect(-view_dir, normal);

	float NoV = abs(dot(normal, view_dir)) + 1e-5;
	// float VoH = dot(view_dir, H);
	// float NoL = clamp(dot(normal, light_dir), 0.0, 1.0);
	// float NoH = clamp(dot(normal, H), 0.0, 1.0);
	// float LoH = clamp(dot(light_dir, H), 0.0, 1.0);

	// Replace 0.04 with 0.16 * reflectance^2
	// here we are using reflectance = 0.5 (for common materials)
	vec3 f0 = mix(vec3(0.04), albedo, metallic);

	// vec3 F = F_Schlick(LoH, f0, 1.0);
	// float V = V_SmithGGXCorrelated(NoV, NoL, alpha);
	// float D = D_GGX(NoH, alpha);
	// vec3 f_r = (V * D) * F;

	// vec3 f_d = (vec3(1.0) - F) * (1 - metallic) * diffuse_color / PI;
	vec3 f_d = (1 - metallic) * texture(u_irradmap, normal).rgb;

	vec3 diffuse = f_d * albedo;

	vec2 dfg_val = texture(u_dfglut, vec2(NoV, roughness)).rg;
	vec3 specular = textureLod(u_radmap, R, 4.0 * roughness).rgb * (f0 * dfg_val.r + dfg_val.g);

	// COLOR = vec4( albedo * 0.04 + (f_d + f_r) * NoL * test_light_power * falloff_coeff/ (4 * PI), 1.0);
	COLOR = vec4(ENV_MULTIPLIER * (diffuse + specular), 1.0);
}

float D_GGX(float NoH, float alpha)
{
	float a = NoH * alpha;
    float k = alpha / (1.0 - NoH * NoH + a * a);
    return k * k * (1.0 / PI);
}


float V_SmithGGXCorrelated(float NoV, float NoL, float alpha) {
    float a2 = alpha * alpha;
    float GGXV = NoL * sqrt(NoV * NoV * (1.0 - a2) + a2);
    float GGXL = NoV * sqrt(NoL * NoL * (1.0 - a2) + a2);
    return 0.5 / (GGXV + GGXL);
}

float V_SmithGGXCorrelatedFast(float NoV, float NoL, float alpha) {
    float a = alpha;
    float GGXV = NoL * (NoV * (1.0 - a) + a);
    float GGXL = NoV * (NoL * (1.0 - a) + a);
    return 0.5 / (GGXV + GGXL);
}

// From Filament
vec3 F_Schlick_opt(float VoH, vec3 f0) 
{
    float f = pow(1.0 - VoH, 5.0);
    return f + f0 * (1.0 - f);
}

vec3 F_Schlick(float VoH, vec3 f0, float f90)
{
	return (f0 + (vec3(f90) - f0) * pow(1.0 - VoH, 5.0));
}
