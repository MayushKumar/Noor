#include "model.h"
#include "core.h"

#include<tiny_gltf.h>
#include <stb_image.h>

namespace Noor
{
	
	Ref<Model> load_model(const char* file_path)
	{
		Ref<Model> model = CreateRef<Model>();

		std::string path(file_path);
		std::string err, warn;

		tinygltf::TinyGLTF loader;

		loader.SetPreserveImageChannels(true);
		stbi_set_flip_vertically_on_load(true);

		tinygltf::Model tiny_model;

		loader.LoadASCIIFromFile(&tiny_model, &err, &warn, path);
		if (err.size()) { NOOR_CORE_ERROR("{0}", err); return nullptr; }
		if (warn.size()) { NOOR_CORE_WARN("{0}", warn); }

		tinygltf::Scene &tiny_scene = tiny_model.scenes[tiny_model.defaultScene];
		for (uint32_t i = 0; i < tiny_scene.nodes.size(); i++)
		{
			tinygltf::Node &tiny_node = tiny_model.nodes[tiny_scene.nodes[i]];
			tinygltf::Mesh &tiny_mesh = tiny_model.meshes[tiny_node.mesh];

			for (uint32_t i = 0; i < tiny_mesh.primitives.size(); i++)
			{
				Ref<Mesh> mesh = CreateRef<Mesh>();

				tinygltf::Primitive &tiny_primitive = tiny_mesh.primitives[i];

				mesh->vertex_array = create_vertex_array();

				// Indices
				tinygltf::Accessor &tiny_indices_acc = tiny_model.accessors[tiny_primitive.indices];
				tinygltf::BufferView &tiny_indices_buffer_view = tiny_model.bufferViews[tiny_indices_acc.bufferView];
				tinygltf::Buffer &tiny_indices_buffer = tiny_model.buffers[tiny_indices_buffer_view.buffer];
				mesh->index_buffer = create_index_buffer(tiny_indices_buffer.data.data()
														 + tiny_indices_acc.byteOffset
														 + tiny_indices_buffer_view.byteOffset,
														 data_type_size(tiny_indices_acc.componentType)
														 * tiny_indices_acc.count,
														 tiny_indices_acc.componentType);
				attach_index_buffer(mesh->vertex_array, mesh->index_buffer);

				// Vertices
				tinygltf::Accessor &tiny_positions_acc = tiny_model.accessors[tiny_primitive.attributes["POSITION"]];
				tinygltf::BufferView &tiny_positions_buffer_view = tiny_model.bufferViews[tiny_positions_acc.bufferView];
				tinygltf::Buffer &tiny_positions_buffer = tiny_model.buffers[tiny_positions_buffer_view.buffer];
				mesh->positions_buffer = create_vertex_buffer(tiny_positions_buffer.data.data()
														   + tiny_positions_acc.byteOffset
														   + tiny_positions_buffer_view.byteOffset,
														   tiny_positions_buffer_view.byteLength,
														   {{"POSITION", NOOR_FLOAT, 3, 0}});
				attach_vertex_buffer(mesh->vertex_array, mesh->positions_buffer, 0);

				tinygltf::Accessor &tiny_normals_acc = tiny_model.accessors[tiny_primitive.attributes["NORMAL"]];
				tinygltf::BufferView &tiny_normals_buffer_view = tiny_model.bufferViews[tiny_normals_acc.bufferView];
				tinygltf::Buffer &tiny_normals_buffer = tiny_model.buffers[tiny_normals_buffer_view.buffer];
				mesh->normals_buffer = create_vertex_buffer(tiny_normals_buffer.data.data()
														   + tiny_normals_acc.byteOffset
														   + tiny_normals_buffer_view.byteOffset,
														   tiny_normals_buffer_view.byteLength,
															{{"NORMAL", NOOR_FLOAT, 3, 0}});
				attach_vertex_buffer(mesh->vertex_array, mesh->normals_buffer, 1);

				tinygltf::Accessor &tiny_tex_coords_acc = tiny_model.accessors[tiny_primitive.attributes["TEXCOORD_0"]];
				tinygltf::BufferView &tiny_tex_coords_buffer_view = tiny_model.bufferViews[tiny_tex_coords_acc.bufferView];
				tinygltf::Buffer &tiny_tex_coords_buffer = tiny_model.buffers[tiny_tex_coords_buffer_view.buffer];
				mesh->tex_coords_buffer = create_vertex_buffer(tiny_tex_coords_buffer.data.data()
														   + tiny_tex_coords_acc.byteOffset
														   + tiny_tex_coords_buffer_view.byteOffset,
														   tiny_tex_coords_buffer_view.byteLength,
															   {{"TEXCOORDS", NOOR_FLOAT, 2, 0}});
				attach_vertex_buffer(mesh->vertex_array, mesh->tex_coords_buffer, 2);

				// Material
				mesh->material = CreateRef<Material>();
				tinygltf::Material &tiny_material = tiny_model.materials[tiny_mesh.primitives[i].material];
				mesh->material->base_color = glm::vec3((float)tiny_material.pbrMetallicRoughness.baseColorFactor[0],
												 (float)tiny_material.pbrMetallicRoughness.baseColorFactor[1],
												 (float)tiny_material.pbrMetallicRoughness.baseColorFactor[2]);
				mesh->material->metallic_factor = tiny_material.pbrMetallicRoughness.metallicFactor;
				mesh->material->roughness_factor = tiny_material.pbrMetallicRoughness.roughnessFactor;
				
				tinygltf::Image &tiny_base_image = tiny_model.images[tiny_model.textures[tiny_material.pbrMetallicRoughness.baseColorTexture.index].source];
				mesh->material->base_tex = create_texture({1, NOOR_RGB8, NOOR_LINEAR, NOOR_LINEAR, NOOR_REPEAT}, {(uint32_t)tiny_base_image.width, (uint32_t)tiny_base_image.height, 3, tiny_base_image.image.data()});
				tinygltf::Image &tiny_normal_image = tiny_model.images[tiny_model.textures[tiny_material.normalTexture.index].source];
				mesh->material->normal_tex = create_texture({1, NOOR_RGB8, NOOR_LINEAR, NOOR_LINEAR, NOOR_REPEAT}, {(uint32_t)tiny_normal_image.width, (uint32_t)tiny_normal_image.height, 3, tiny_normal_image.image.data()});
				tinygltf::Image &tiny_metallic_roughness_image = tiny_model.images[tiny_model.textures[tiny_material.pbrMetallicRoughness.metallicRoughnessTexture.index].source];
				mesh->material->metallic_roughness_tex = create_texture({1, NOOR_RGB8, NOOR_LINEAR, NOOR_LINEAR, NOOR_REPEAT}, {(uint32_t)tiny_metallic_roughness_image.width, (uint32_t)tiny_metallic_roughness_image.height, 3, tiny_metallic_roughness_image.image.data()});
				
				model->meshes.push_back(mesh);
			}
		}

		return model;
	}

	void delete_model(Ref<Model> model)
	{
		for(Ref<Mesh> &mesh : model->meshes)
		{
			delete_vertex_array(mesh->vertex_array);
			delete_vertex_buffer(mesh->positions_buffer);
			delete_vertex_buffer(mesh->normals_buffer);
			delete_vertex_buffer(mesh->tex_coords_buffer);
			delete_index_buffer(mesh->index_buffer);

			delete_texture(mesh->material->base_tex);
			delete_texture(mesh->material->normal_tex);
			delete_texture(mesh->material->metallic_roughness_tex);
		}
	}

}
