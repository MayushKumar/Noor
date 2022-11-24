#pragma once

#include "buffers.h"
#include "texture.h"

#include <glm/glm.hpp>

namespace Noor
{

	struct Material
	{
		glm::vec3 base_color;
		float metallic_factor;
		float roughness_factor;

		Ref<Texture2D> base_tex;
		Ref<Texture2D> normal_tex;
		Ref<Texture2D> metallic_roughness_tex;
	};
	
	struct Mesh
	{
		Ref<IndexBuffer> index_buffer;
		
		Ref<VertexBuffer> positions_buffer;
		Ref<VertexBuffer> normals_buffer;
		Ref<VertexBuffer> tex_coords_buffer;

		Ref<VertexArray> vertex_array;

		Ref<Material> material;
	};

	struct Model
	{
		std::vector<Ref<Mesh>> meshes;
	};

	Ref<Model> load_model(const char* file_path);
	void delete_model(Ref<Model> model);

}
