#version 450 core

in vec3 v_dir;
in vec3 v_right;

out vec4 COLOR;

uniform samplerCube u_envmap;
uniform float u_roughness;

#define PI 3.14159265358979

const int N = 500;

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float alpha);
float GeometrySmith(float roughness, float NoV, float NoL);
float GeometrySchlickGGX(float NdotV, float roughness);
vec2 IntegrateBRDF(float NoV, float roughness);
float VdC(uint bits);
vec2 Hammersley2D(uint i, uint N);

void main()
{
	vec3 dir = normalize(v_dir);
	vec3 bitangent = normalize(cross(dir, v_right));
	vec3 tangent = normalize(cross(dir, bitangent));
	vec3 val = vec3(0.0);
	uint i = 0;
	float total_weight = 0;
	while(i < N)
	{
		vec2 Xi = Hammersley2D(i, N);
		vec3 H = ImportanceSampleGGX(Xi, dir, u_roughness*u_roughness);
		vec3 L = 2 * dot(dir, H) * H - dir;
		float NoL = clamp(dot(L, dir), 0.0, 1.0);
		if(NoL > 0.0)
		{
			val += clamp(textureLod(u_envmap, L, 0.0).rgb, vec3(0.0), vec3(20.0)) * NoL;
			total_weight += NoL;
		}
		i++;
	}
	COLOR = vec4(val / total_weight, 1.0);
	// COLOR = vec4(u_roughness, 1-u_roughness, 0.0, 1.0);
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

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float alpha)
{
    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (alpha*alpha - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
	
    // from spherical coordinates to cartesian coordinates
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;
	
    // from tangent-space vector to world-space sample vector
    vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent   = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);
	
    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
} 

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float a = roughness;
    float k = (a * a) / 2.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(float roughness, float NoV, float NoL)
{
    float ggx2 = GeometrySchlickGGX(NoV, roughness);
    float ggx1 = GeometrySchlickGGX(NoL, roughness);

    return ggx1 * ggx2;
}  
