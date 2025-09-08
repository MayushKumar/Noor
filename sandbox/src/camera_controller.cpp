#include "camera_controller.h"

#include <glm/gtc/quaternion.hpp>

static const float c_TRANS_SPEED = 0.015f, c_ROTATION_SPEED = 0.001f, c_ROTATION_DRAG = 0.4f;

CameraController::CameraController(Noor::Window::WindowHandle window_handle, float fov, float aspect_ratio, float near, float far)
{
	m_window_handle = window_handle;
	m_camera = Noor::CreateRef<Noor::Camera>();
	m_camera->fov = fov;
	m_camera->aspect_ratio = aspect_ratio;
	m_camera->near = near;
	m_camera->far = far;
	m_camera->position = glm::vec3(0.0f, 0.0f, 1.0f);
	m_camera->orientation = glm::quatLookAtRH(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Noor::calculate_camera_matrices(m_camera);
}

void CameraController::update()
{
	if (!m_is_enabled) return;

	glm::vec2 delta_mouse_pos = m_current_mouse_pos - m_drag_mouse_pos;

	m_drag_mouse_pos += delta_mouse_pos * c_ROTATION_DRAG;

	m_yaw += -delta_mouse_pos.x * c_ROTATION_SPEED - m_right_joystick_x * 0.06f;
	m_pitch += -delta_mouse_pos.y * c_ROTATION_SPEED - m_right_joystick_y * 0.06f;
	m_pitch = glm::clamp(m_pitch, glm::radians(-90.0f), glm::radians(90.0f));

	m_camera->orientation = glm::quat(glm::vec3(m_pitch, m_yaw, 0.0f));

	m_camera->position += m_camera->orientation * m_pos_delta * c_TRANS_SPEED;


	Noor::calculate_camera_view_matrix(m_camera);
}

glm::mat4& CameraController::get_view_proj_matrix()
{
	return m_camera->view_proj_mat;
}

glm::mat4& CameraController::get_proj_matrix()
{
	return m_camera->proj_mat;
}

glm::quat& CameraController::get_orientation()
{
	return m_camera->orientation;
}

glm::vec3& CameraController::get_position()
{
	return m_camera->position;
}

void CameraController::key_event(uint32_t key, uint32_t action, uint32_t mods)
{
	if (!m_is_enabled) return;

	static glm::quat s_pitch_up = glm::quat(glm::vec3(glm::radians(30.0f), 0.0f, 0.0f));
	static glm::quat s_pitch_down = glm::quat(glm::vec3(glm::radians(-30.0f), 0.0f, 0.0f));
	static glm::quat s_yaw_left = glm::quat(glm::vec3(0.0f, glm::radians(30.0f), 0.0f));
	static glm::quat s_yaw_right = glm::quat(glm::vec3(0.0f, glm::radians(-30.0f), 0.0f));

	if (action == NOOR_PRESSED)
	{
		switch (key) {
		case NOOR_KEY_W:
			m_pos_delta += glm::vec3(0.0f, 0.0f, -1.0f); break;
		case NOOR_KEY_A:
			m_pos_delta += glm::vec3(-1.0f, 0.0f, 0.0f); break;
		case NOOR_KEY_S:
			m_pos_delta += glm::vec3(0.0f, 0.0f, 1.0f); break;
		case NOOR_KEY_D:
			m_pos_delta += glm::vec3(1.0f, 0.0f, 0.0f); break;
		case NOOR_KEY_UP:
			m_camera->orientation = m_camera->orientation * s_pitch_up; break;
		case NOOR_KEY_DOWN:
			m_camera->orientation = m_camera->orientation * s_pitch_down; break;
		case NOOR_KEY_LEFT:
			m_camera->orientation = m_camera->orientation * s_yaw_left; break;
		case NOOR_KEY_RIGHT:
			m_camera->orientation = m_camera->orientation * s_yaw_right; break;
		}
	}

	if (action == NOOR_RELEASED)
	{
		switch (key) {
		case NOOR_KEY_W:
			m_pos_delta -= glm::vec3(0.0f, 0.0f, -1.0f); break;
		case NOOR_KEY_A:
			m_pos_delta -= glm::vec3(-1.0f, 0.0f, 0.0f); break;
		case NOOR_KEY_S:
			m_pos_delta -= glm::vec3(0.0f, 0.0f, 1.0f); break;
		case NOOR_KEY_D:
			m_pos_delta -= glm::vec3(1.0f, 0.0f, 0.0f); break;
		}
	}
}

void CameraController::mouse_position_event(int32_t x, int32_t y)
{
	if (!m_is_enabled) return;

	m_current_mouse_pos = glm::vec2(x, y);
}

void CameraController::right_joystick_xy(float x, float y)
{
	m_right_joystick_x = x;
	m_right_joystick_y = y;
}

void CameraController::set_enabled(bool enabled)
{
	m_is_enabled = enabled;

    // Disable cursor when controller is enabled
	Noor::Window::set_cursor_enabled(m_window_handle, !enabled);

	if (enabled)
	{
		int32_t x, y;
		Noor::Window::get_cursor_position(m_window_handle, x, y);
		m_drag_mouse_pos = m_current_mouse_pos = glm::vec2(x, y);
	}
}
