#include "model.h"
#include "buffers.h"
#include "defs.h"
#include "log.h"

#include <sys/types.h>
#include <tiny_gltf.h>
#include <stb_image.h>
#include <glm/gtc/type_ptr.hpp>

namespace Noor
{

	uint32_t get_tinygltf_type_size(uint32_t type); // defined in tiny_gltf.cpp

	Ref<Model> load_model(const char* file_path)
	{
		Ref<Model> model = CreateRef<Model>();

		std::string path(file_path);
		std::string err, warn;

		tinygltf::TinyGLTF loader;

		loader.SetPreserveImageChannels(true);
		stbi_set_flip_vertically_on_load(true);

		tinygltf::Model tiny_model;

		NOOR_CORE_TRACE("Loading model: {}", file_path);

		loader.LoadASCIIFromFile(&tiny_model, &err, &warn, path);
		if (err.size()) { NOOR_CORE_ERROR("{0}", err); return nullptr; }
		if (warn.size()) { NOOR_CORE_WARN("{0}", warn); }

		tinygltf::Scene &tiny_scene = tiny_model.scenes[tiny_model.defaultScene];
		// NOOR_CORE_TRACE("\tNode count: {}", tiny_scene.nodes.size());
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
														 get_tinygltf_type_size(tiny_indices_acc.componentType)
														 * tiny_indices_acc.count,
														 gl_data_type_to_data_type(tiny_indices_acc.componentType));
				attach_index_buffer(mesh->vertex_array, mesh->index_buffer);

				// Vertices
				tinygltf::Accessor &tiny_positions_acc = tiny_model.accessors[tiny_primitive.attributes["POSITION"]];
				tinygltf::BufferView &tiny_positions_buffer_view = tiny_model.bufferViews[tiny_positions_acc.bufferView];
				tinygltf::Buffer &tiny_positions_buffer = tiny_model.buffers[tiny_positions_buffer_view.buffer];
				mesh->positions_buffer = create_vertex_buffer(tiny_positions_buffer.data.data()
															  + tiny_positions_acc.byteOffset
															  + tiny_positions_buffer_view.byteOffset,
															  tiny_positions_buffer_view.byteLength,
															  {{"POSITION", DataType::F32, 3, 0}});
				attach_vertex_buffer(mesh->vertex_array, mesh->positions_buffer, 0);

				tinygltf::Accessor &tiny_normals_acc = tiny_model.accessors[tiny_primitive.attributes["NORMAL"]];
				tinygltf::BufferView &tiny_normals_buffer_view = tiny_model.bufferViews[tiny_normals_acc.bufferView];
				tinygltf::Buffer &tiny_normals_buffer = tiny_model.buffers[tiny_normals_buffer_view.buffer];
				mesh->normals_buffer = create_vertex_buffer(tiny_normals_buffer.data.data()
															+ tiny_normals_acc.byteOffset
															+ tiny_normals_buffer_view.byteOffset,
															tiny_normals_buffer_view.byteLength,
															{{"NORMAL", DataType::F32, 3, 0}});
				attach_vertex_buffer(mesh->vertex_array, mesh->normals_buffer, 1);

				tinygltf::Accessor &tiny_tangents_acc = tiny_model.accessors[tiny_primitive.attributes["TANGENT"]];
				tinygltf::BufferView &tiny_tangents_buffer_view = tiny_model.bufferViews[tiny_tangents_acc.bufferView];
				tinygltf::Buffer &tiny_tangents_buffer = tiny_model.buffers[tiny_tangents_buffer_view.buffer];
				mesh->tangents_buffer = create_vertex_buffer(tiny_tangents_buffer.data.data()
															+ tiny_tangents_acc.byteOffset
															+ tiny_tangents_buffer_view.byteOffset,
															tiny_tangents_buffer_view.byteLength,
															 {{"TANGENT", DataType::F32, 4, 0}});
				attach_vertex_buffer(mesh->vertex_array, mesh->tangents_buffer, 2);

				tinygltf::Accessor &tiny_tex_coords_acc = tiny_model.accessors[tiny_primitive.attributes["TEXCOORD_0"]];
				tinygltf::BufferView &tiny_tex_coords_buffer_view = tiny_model.bufferViews[tiny_tex_coords_acc.bufferView];
				tinygltf::Buffer &tiny_tex_coords_buffer = tiny_model.buffers[tiny_tex_coords_buffer_view.buffer];
				mesh->tex_coords_buffer = create_vertex_buffer(tiny_tex_coords_buffer.data.data()
															   + tiny_tex_coords_acc.byteOffset
															   + tiny_tex_coords_buffer_view.byteOffset,
															   tiny_tex_coords_buffer_view.byteLength,
															   {{"TEXCOORDS", DataType::F32, 2, 0}});
				attach_vertex_buffer(mesh->vertex_array, mesh->tex_coords_buffer, 3);

				// Material
				mesh->material = CreateRef<Material>();
				tinygltf::Material &tiny_material = tiny_model.materials[tiny_mesh.primitives[i].material];
				mesh->material->base_color = glm::vec3((float)tiny_material.pbrMetallicRoughness.baseColorFactor[0],
													   (float)tiny_material.pbrMetallicRoughness.baseColorFactor[1],
													   (float)tiny_material.pbrMetallicRoughness.baseColorFactor[2]);
				mesh->material->metallic_factor = tiny_material.pbrMetallicRoughness.metallicFactor;
				mesh->material->roughness_factor = tiny_material.pbrMetallicRoughness.roughnessFactor;
				
				if (tiny_material.pbrMetallicRoughness.baseColorTexture.index != -1)
				{
					tinygltf::Image &tiny_base_image = tiny_model.images[tiny_model.textures[tiny_material.pbrMetallicRoughness.baseColorTexture.index].source];
					Util::Image base_image = Util::flip_image_vertically({(uint32_t)tiny_base_image.width, (uint32_t)tiny_base_image.height, DataType::U8, (uint32_t)tiny_base_image.component, tiny_base_image.image.data()});
					mesh->material->base_tex = create_texture({0, 0, TextureFormat::SRGB8, 0, NOOR_LINEAR, NOOR_LINEAR, NOOR_REPEAT}, base_image);
					Util::free_image(base_image);
				}
				
				if (tiny_material.normalTexture.index != -1)
				{
					tinygltf::Image &tiny_normal_image = tiny_model.images[tiny_model.textures[tiny_material.normalTexture.index].source];
					Util::Image normal_image = Util::flip_image_vertically({(uint32_t)tiny_normal_image.width, (uint32_t)tiny_normal_image.height, DataType::U8, (uint32_t)tiny_normal_image.component, tiny_normal_image.image.data()});
					mesh->material->normal_tex = create_texture({0, 0, TextureFormat::RGB8, 0, NOOR_LINEAR, NOOR_LINEAR, NOOR_REPEAT}, normal_image); 
					Util::free_image(normal_image);
				}

				if (tiny_material.pbrMetallicRoughness.metallicRoughnessTexture.index != -1)
				{
					tinygltf::Image &tiny_metallic_roughness_image = tiny_model.images[tiny_model.textures[tiny_material.pbrMetallicRoughness.metallicRoughnessTexture.index].source];
					Util::Image metallic_roughness_image = Util::flip_image_vertically({(uint32_t)tiny_metallic_roughness_image.width, (uint32_t)tiny_metallic_roughness_image.height, DataType::U8, (uint32_t)tiny_metallic_roughness_image.component, tiny_metallic_roughness_image.image.data()});
					mesh->material->metallic_roughness_tex = create_texture({0, 0, TextureFormat::RGB8, 0, NOOR_LINEAR, NOOR_LINEAR, NOOR_REPEAT},  metallic_roughness_image);
					Util::free_image(metallic_roughness_image);
				}

				if (tiny_node.translation.size())
				{
					mesh->translation = glm::vec3((float)tiny_node.translation[0],
												  (float)tiny_node.translation[1],
												  (float)tiny_node.translation[2]);
				}

				#if 0

				uint16_t* indices_data = (uint16_t*)(tiny_indices_buffer.data.data()
													 + tiny_indices_acc.byteOffset
													 + tiny_indices_buffer_view.byteOffset);
				float* position_data = (float*)(tiny_positions_buffer.data.data()
												+ tiny_positions_acc.byteOffset
												+ tiny_positions_buffer_view.byteOffset);
				float* normal_data = (float*)(tiny_normals_buffer.data.data()
											  + tiny_normals_acc.byteOffset
											  + tiny_normals_buffer_view.byteOffset);
				float* texcoord_data = (float*)(tiny_tex_coords_buffer.data.data()
												+ tiny_tex_coords_acc.byteOffset
												+ tiny_tex_coords_buffer_view.byteOffset);

				// float* tangent_data = (float*)std::calloc(tiny_positions_buffer.data.size(), 1);
				// float* bitangent_data = (float*)std::calloc(tiny_positions_buffer.data.size(), 1);
				float* tangent_data = (float*)std::calloc(tiny_positions_buffer.data.size() / sizeof(float), sizeof(float));
				float* bitangent_data = (float*)std::calloc(tiny_positions_buffer.data.size() / sizeof(float), sizeof(float));

				NOOR_CORE_TRACE("indices count: {0}", tiny_indices_acc.count);
				
				for(uint32_t i = 0; i < tiny_indices_acc.count; i += 3)
				{
					
					uint16_t i1 = indices_data[i];
					uint16_t i2 = indices_data[i + 1];
					uint16_t i3 = indices_data[i + 2];

					uint32_t ii1 = (uint32_t)i1 * 3;
					uint32_t ii2 = (uint32_t)i2 * 3;
					uint32_t ii3 = (uint32_t)i3 * 3;

					glm::vec3 e1(
						position_data[ii2] - position_data[ii1],
						position_data[ii2 + 1] - position_data[ii1 + 1],
						position_data[ii2 + 2] - position_data[ii1 + 2]
						);
					glm::vec3 e2(
						position_data[ii3] - position_data[ii1],
						position_data[ii3 + 1] - position_data[ii1 + 1],
						position_data[ii3 + 2] - position_data[ii1 + 2]
						);
					glm::vec2 tce1(
						texcoord_data[(uint32_t)i2 * 2] - texcoord_data[(uint32_t)i1 * 1],
						texcoord_data[(uint32_t)i2 * 2 + 1] - texcoord_data[(uint32_t)i1 * 2 + 1]
						);
					glm::vec2 tce2(
						texcoord_data[(uint32_t)i3 * 2] - texcoord_data[(uint32_t)i1 * 2],
						texcoord_data[(uint32_t)i3 * 2 + 1] - texcoord_data[(uint32_t)i1 * 2 + 1]
						);
					 
					glm::mat2x2 inv = glm::inverse(glm::mat2x2(tce1, tce2));
					glm::mat2x3 tbn = glm::mat2x3(e1, e2) * inv;

					tbn[0] = glm::normalize(tbn[0]);
					tbn[1] = glm::normalize(tbn[1]);

					std::memcpy(&tangent_data[ii1], glm::value_ptr(tbn[0]), 3 * sizeof(float));
					std::memcpy(&tangent_data[ii2], glm::value_ptr(tbn[0]), 3 * sizeof(float));
					std::memcpy(&tangent_data[ii3], glm::value_ptr(tbn[0]), 3 * sizeof(float));
					std::memcpy(&bitangent_data[ii1], glm::value_ptr(tbn[1]), 3 * sizeof(float));
					std::memcpy(&bitangent_data[ii2], glm::value_ptr(tbn[1]), 3 * sizeof(float));
					std::memcpy(&bitangent_data[ii3], glm::value_ptr(tbn[1]), 3 * sizeof(float));
				}

				mesh->tangents_buffer = create_vertex_buffer((char*)tangent_data, tiny_positions_buffer_view.byteLength, {{"TANGENT", NOOR_FLOAT, 3, 0}});
				attach_vertex_buffer(mesh->vertex_array, mesh->tangents_buffer, 3);
				mesh->bitangents_buffer = create_vertex_buffer((char*)bitangent_data, tiny_positions_buffer_view.byteLength, {{"BITANGENT", NOOR_FLOAT, 3, 0}});
				attach_vertex_buffer(mesh->vertex_array, mesh->bitangents_buffer, 4);

				std::free(tangent_data);
				std::free(bitangent_data);

				#endif

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
