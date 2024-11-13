#include "buffers.h"
#include "defs.h"

#include <glad/glad.h>

namespace Noor
{
	uint32_t PrimitiveType_to_gl(PrimitiveType type);

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
		vertex_array->primitive_type = PrimitiveType::TRIANGLE;
		glCreateVertexArrays(1, &vertex_array->id);
		return vertex_array;
	}

	void bind_vertex_array(const Ref<VertexArray> vertex_array)
	{
		glBindVertexArray(vertex_array->id);
	}

	void attach_vertex_buffer(Ref<VertexArray> vertex_array, Ref<VertexBuffer> vertex_buffer, uint32_t attrib_offset)
	{
		glVertexArrayVertexBuffer(vertex_array->id, attrib_offset, vertex_buffer->id, 0, vertex_buffer->stride);

		for (uint32_t i = 0; i < vertex_buffer->elements.size(); i++)
		{
			VertexBufferElement &element = vertex_buffer->elements[i];
			glEnableVertexArrayAttrib(vertex_array->id, i + attrib_offset);
			glVertexArrayAttribFormat(vertex_array->id, i + attrib_offset, element.count, data_type_to_gl(element.data_type), false, element.offset);
			glVertexArrayAttribBinding(vertex_array->id, i + attrib_offset, attrib_offset); 
		}

		vertex_array->vertex_buffers.push_back(vertex_buffer);
	}

	void attach_index_buffer(Ref<VertexArray> vertex_array, const Ref<IndexBuffer> index_buffer)
	{
		glVertexArrayElementBuffer(vertex_array->id, index_buffer->id);
		vertex_array->index_buffer = index_buffer;
	}

	void set_primitive_type(Ref<VertexArray> vertex_array, PrimitiveType primitive_type)
	{
		vertex_array->primitive_type = primitive_type;
	}

	void draw_indexed(const Ref<VertexArray> vertex_array, uint32_t indices_count)
	{
		glBindVertexArray(vertex_array->id);
		glDrawElements(PrimitiveType_to_gl(vertex_array->primitive_type),
					   indices_count ? indices_count : vertex_array->index_buffer->count,
					   data_type_to_gl(vertex_array->index_buffer->data_type) , nullptr);
	}

	void delete_vertex_buffer(Ref<VertexBuffer> buffer)
	{
		glDeleteBuffers(1, &buffer->id);
	}

	void delete_index_buffer(Ref<IndexBuffer> buffer)
	{
		glDeleteBuffers(1, &buffer->id);
	}

	void delete_vertex_array(Ref<VertexArray> vertex_array)
	{
		glDeleteVertexArrays(1, &vertex_array->id);
	}

	uint32_t PrimitiveType_to_gl(PrimitiveType type)
	{
		switch (type)
		{
		case PrimitiveType::TRIANGLE: return GL_TRIANGLES;
		case PrimitiveType::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
		}
	}
}
