#include "buffers.h"

#include <glad/glad.h>

namespace Noor
{

	Ref<VertexBuffer> create_vertex_buffer(void* data, uint32_t size, std::vector<VertexBufferElement> elements)
	{
		Ref<VertexBuffer> buffer = CreateRef<VertexBuffer>();
		buffer->size = size;
		buffer->elements = elements;
		for (VertexBufferElement &element : elements)
		{
			buffer->stride += data_type_size(element.data_type) * element.count;
		}
		glCreateBuffers(1, &buffer->id);
		glNamedBufferData(buffer->id, size, data, GL_STATIC_DRAW);
		return buffer;
	}

	Ref<IndexBuffer> create_index_buffer(void* data, uint32_t size, DataType data_type)
	{
		Ref<IndexBuffer> buffer = CreateRef<IndexBuffer>();
		buffer->size = size;
		buffer->data_type = data_type;
		buffer->count = size / data_type_size(data_type);
		glCreateBuffers(1, &buffer->id);
		glNamedBufferData(buffer->id, size, data, GL_STATIC_DRAW);
		return buffer;
	}
	
	Ref<VertexArray> create_vertex_array()
	{
		Ref<VertexArray> vertex_array = CreateRef<VertexArray>();
		glCreateVertexArrays(1, &vertex_array->id);
		return vertex_array;
	}

	void bind_vertex_array(const Ref<VertexArray>& vertex_array)
	{
		glBindVertexArray(vertex_array->id);
	}

	void attach_vertex_buffer(Ref<VertexArray>& vertex_array, const Ref<VertexBuffer>& vertex_buffer, uint32_t attrib_offset)
	{
		glVertexArrayVertexBuffer(vertex_array->id, attrib_offset, vertex_buffer->id, 0, vertex_buffer->stride);

		for (uint32_t i = 0; i < vertex_buffer->elements.size(); i++)
		{
			VertexBufferElement &element = vertex_buffer->elements[i];
			glVertexArrayAttribFormat(vertex_array->id, i, element.count, element.data_type, false, element.offset);
			glEnableVertexArrayAttrib(vertex_array->id, i);
			glVertexArrayAttribBinding(vertex_array->id, i, attrib_offset); 
		}

		vertex_array->vertex_buffers.push_back(vertex_buffer);
	}

	void attach_index_buffer(Ref<VertexArray>& vertex_array, const Ref<IndexBuffer>& index_buffer)
	{
		glVertexArrayElementBuffer(vertex_array->id, index_buffer->id);
		vertex_array->index_buffer = index_buffer;
	}

	void draw_indexed(const Ref<VertexArray>& vertex_array, uint32_t indices_count)
	{
		glBindVertexArray(vertex_array->id);
		glDrawElements(GL_TRIANGLES,
					   indices_count ? indices_count : vertex_array->index_buffer->count,
					   vertex_array->index_buffer->data_type, nullptr);
	}
}
