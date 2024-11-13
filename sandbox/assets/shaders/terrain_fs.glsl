#version 450 core

in vec3 v_normal;
in vec3 v_view_dir;

out vec4 COLOR;

uniform samplerCube u_env_map;

vec3 BLUE = vec3(0, 0.002, 0.4);
//============================== FORWARD DECLARATIONS ============================== 
vec3 F_Schlick_opt(float VoH, vec3 f0);
//============================== ==================== ============================== 

void main() {
	vec3 N = normalize(v_normal);
	vec3 V = normalize(v_view_dir);
	// vec3 H = normalize(N + V);
	vec3 R = reflect(-V, N);

	vec3 reflection = pow(texture(u_env_map, R).rgb, vec3(1.0));
	vec3 F = F_Schlick_opt(dot(V, N), vec3(0.01));

	COLOR = vec4(mix(BLUE, reflection, F), 1.0);
	// COLOR = vec4(reflection, 1.0);
}


vec3 F_Schlick_opt(float VoH, vec3 f0) 
{
    float f = pow(1.0 - VoH, 5.0);
    return f + f0 * (1.0 - f);
}
