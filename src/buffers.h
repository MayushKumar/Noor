#include "core.h"

namespace Noor
{

	struct VertexBufferElement
	{
		const char* name;
		DataType dataType;
		uint32_t offset;
		uint32_t stride;
	};

	struct VertexBuffer
	{
		uint32_t id;
		uint32_t size;
		std::vector<VertexBufferElement> elements;
	};

	struct IndexBuffer
	{
		uint32_t id;
		uint32_t size; 
	};

	VertexBuffer createVertexBuffer(void* data, uint32_t size, std::vector<VertexBufferElement> elements);
	IndexBuffer createIndexBuffer(void* data, uint32_t size, DataType dataType);
}
