#include <Noor.h>

class CameraController
{

public:
	CameraController() {}
	CameraController(Noor::Window::WindowHandle window_handle, float fov, float aspect_ratio, float near, float far);

	void update();
	void key_event(uint32_t key, uint32_t action, uint32_t mods);
	void mouse_position_event(int32_t x, int32_t y);
	void right_joystick_xy(float x, float y);
	void set_enabled(bool enabled);

	bool is_enabled() { return m_is_enabled; };

	glm::mat4& get_view_proj_matrix();
	glm::mat4& get_proj_matrix();
	glm::quat& get_orientation();
	glm::vec3& get_position();

private:
	Noor::Ref<Noor::Camera> m_camera;
	Noor::Window::WindowHandle m_window_handle;
	float m_right_joystick_x = 0;
	float m_right_joystick_y = 0;
	glm::vec3 m_pos_delta = glm::vec3(0.0f);
	glm::vec2 m_drag_mouse_pos;
	glm::vec2 m_current_mouse_pos;
	float m_yaw, m_pitch = 0.0f;
	bool m_is_enabled = false;
};
