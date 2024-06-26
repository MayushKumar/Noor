#pragma once

#include <vector>
#include "defs.h"

namespace Noor
{

	struct VertexBufferElement
	{
		const char* name;
		DataType data_type;
		uint32_t count;
		uint32_t offset;
	};

	struct VertexBuffer
	{
		uint32_t id;
		uint32_t size;
		uint32_t stride;
		std::vector<VertexBufferElement> elements;
	};

	struct IndexBuffer
	{
		uint32_t id;
		uint32_t count;
		uint32_t size; 
		DataType data_type;
	};

	struct VertexArray
	{
		uint32_t id;
		std::vector<Ref<VertexBuffer>> vertex_buffers;
		Ref<IndexBuffer> index_buffer;
	};

	Ref<VertexBuffer> create_vertex_buffer(void* data, uint32_t size, std::vector<VertexBufferElement> elements);
	Ref<IndexBuffer> create_index_buffer(void* data, uint32_t size, DataType data_type);
	Ref<VertexArray> create_vertex_array();

	void bind_vertex_array(const Ref<VertexArray>& vertex_array);
	void attach_vertex_buffer(Ref<VertexArray>& vertex_array, const Ref<VertexBuffer>& vertex_buffer, uint32_t attrib_offset);
	void attach_index_buffer(Ref<VertexArray>& vertex_array, const Ref<IndexBuffer>& index_buffer);

	void delete_vertex_buffer(Ref<VertexBuffer> buffer);
	void delete_index_buffer(Ref<IndexBuffer> buffer);
	void delete_vertex_array(Ref<VertexArray> vertex_array);

	void draw_indexed(const Ref<VertexArray>& vertex_array, uint32_t indices_count = 0);
}
