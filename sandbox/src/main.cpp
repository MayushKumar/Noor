#include <iostream>

#include "Noor.h"

Noor::Window::WindowProps windowProps;
Noor::Window::WindowHandle windowHandle;

void gameLoop();

Noor::VertexBuffer vertex_buffer;
Noor::IndexBuffer index_buffer;

int main()
{
	windowProps.width = 1280;
	windowProps.height = 720;
	windowHandle = Noor::Window::createWindow(windowProps);
	Noor::init(windowHandle);
	gameLoop();

	float vertices [3 * 3] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	uint32_t indices [3] = {0, 1, 2};

	vertex_buffer = Noor::createVertexBuffer((void*)vertices, sizeof(vertices), {{"Positions", NOOR_FLOAT, 0, sizeof(float) * 3 }});
	index_buffer = Noor::createIndexBuffer((void*)indices, sizeof(indices), NOOR_UINT);
	
}

void gameLoop()
{
	while(!Noor::Window::windowShouldClose(windowHandle))
	{
		Noor::Window::pollEvents();
		Noor::Window::swapBuffers(windowHandle);
	}
}
