#version 450 core

in vec2 v_TEXCOORDS;

out vec2 COLOR;

float GeometrySmith(float roughness, float NoV, float NoL);
float GeometrySchlickGGX(float NdotV, float roughness);
vec2 IntegrateBRDF(float NoV, float roughness);
vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float alpha);
vec2 Hammersley2D(uint i, uint N);
float VdC(uint bits);

#define PI 3.14159265358979
const int SAMPLE_COUNT = 1000; 

void main()
{
	// COLOR = texture(u_texture, v_TEXCOORDS);

	COLOR = IntegrateBRDF(v_TEXCOORDS.x, v_TEXCOORDS.y);
}

vec2 IntegrateBRDF(float NoV, float roughness)
{
	vec3 V;
	V.x = sqrt(1.0f - NoV * NoV); // sin
	V.y = 0;
	V.z = NoV; // cos
	float A = 0;
	float B = 0;
	const uint NumSamples = 1024;
	for(uint i = 0; i < NumSamples; i++)
	{
		vec2 Xi = Hammersley2D(i, SAMPLE_COUNT);
		vec3 H = ImportanceSampleGGX(Xi, vec3(0.0, 0.0, 1.0), roughness*roughness);
		vec3 L = 2 * dot(V, H) * H - V;
		float NoL = clamp(L.z, 0.0, 1.0);
		float NoH = clamp(H.z, 0.0, 1.0);
		float VoH = clamp(dot(V, H), 0.0, 1.0);
		if(NoL > 0)
		{
			float G = GeometrySmith(roughness, NoV, NoL);
			float G_Vis = G * VoH / (NoH * NoV);
			float Fc = pow(1 - VoH, 5);
			A += (1 - Fc) * G_Vis;
			B += Fc * G_Vis;
		}
	}
	return vec2(A, B) / SAMPLE_COUNT;
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

vec2 Hammersley2D(uint i, uint N)
{
	return vec2(float(i)/float(N), VdC(i));
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
