#include "buffers.h"

#include <glad/glad.h>

namespace Noor
{

	VertexBuffer createVertexBuffer(void* data, uint32_t size, std::vector<VertexBufferElement> elements)
	{
		VertexBuffer buffer;
		buffer.size = size;
		glCreateBuffers(1, &buffer.id);
		glNamedBufferData(buffer.id, size, data, GL_STATIC_DRAW);
		return buffer;
	}

	IndexBuffer createIndexBuffer(void* data, uint32_t size, DataType dataType)
	{
		IndexBuffer buffer;
		glCreateBuffers(1, &buffer.id);
		glNamedBufferData(buffer.id, size, data, GL_STATIC_DRAW);
		return buffer;
	}
	
}    
