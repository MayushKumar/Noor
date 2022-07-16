#include <Noor.h>

class CameraController
{

public:
	CameraController() {}
	CameraController(Noor::Window::WindowHandle window_handle, float fov, float aspect_ratio, float near, float far);

	void update();
	void key_event(uint32_t key, uint32_t action, uint32_t mods);
	void mouse_position_event(int32_t x, int32_t y);
	void set_enabled(bool enabled);

	bool is_enabled() { return m_is_enabled; };

	glm::mat4& get_view_proj_matrix();

private:
	Noor::Ref<Noor::Camera> m_camera;
	Noor::Window::WindowHandle m_window_handle;
	glm::vec3 m_pos_delta = glm::vec3(0.0f);
	glm::vec2 m_drag_mouse_pos;
	glm::vec2 m_current_mouse_pos;
	float m_yaw, m_pitch = 0.0f;
	bool m_is_enabled = false;
};
