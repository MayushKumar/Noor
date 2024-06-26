#pragma once
#include "defs.h"

#include <glm/gtc/quaternion.hpp>

namespace Noor
{

	struct Camera
	{
		glm::vec3 position;
		glm::quat orientation;
		glm::mat4 view_mat;
		glm::mat4 proj_mat;
		glm::mat4 view_proj_mat;
		float fov, aspect_ratio, near, far;
	};

	void calculate_camera_projection_matrix(Ref<Camera> camera);
	void calculate_camera_view_matrix(Ref<Camera> camera);
	void calculate_camera_matrices(Ref<Camera> camera);
	
}    
