#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Noor
{

	void calculate_camera_projection_matrix(Ref<Camera> camera)
	{
		camera->proj_mat = glm::perspective(camera->fov, camera->aspect_ratio,
												  camera->near, camera->far);
		camera->view_proj_mat = camera->proj_mat * camera->view_mat;
	}

	void calculate_camera_view_matrix(Ref<Camera> camera)
	{
		camera->view_mat = glm::mat4_cast(glm::inverse(camera->orientation))
						 * glm::translate(glm::mat4(1.0f), -camera->position);
		camera->view_proj_mat = camera->proj_mat * camera->view_mat;
	}

	void calculate_camera_matrices(Ref<Camera> camera)
	{
		camera->proj_mat = glm::perspective(camera->fov, camera->aspect_ratio,
												  camera->near, camera->far);
		camera->view_mat = glm::mat4_cast(glm::inverse(camera->orientation))
						 * glm::translate(glm::mat4(1.0f), -camera->position);
		camera->view_proj_mat = camera->proj_mat * camera->view_mat;
	}

}    
