#include "defs.h"
#include <cstdint>
#include <glad/glad.h>

namespace Noor
{
	uint32_t data_type_size(DataType type)
	{
		switch (type)
		{
			case DataType::B8:
			case DataType::I8:             
			case DataType::U8:    return 1;
			case DataType::F16:
			case DataType::I16: 
			case DataType::U16:   return 2;
			case DataType::I32: 
			case DataType::U32:
			case DataType::F32:   return 4;
			case DataType::F64:   return 8;
			// case DataType::F32X2: return 8;
			// case DataType::F32X3: return 12;
			// case DataType::F32X4: return 16;
		}
	}

	uint32_t data_type_to_gl(DataType type)
	{
		switch (type)
		{
			case DataType::B8:    return GL_BOOL;
			case DataType::I8:    return GL_BYTE;
			case DataType::U8:    return GL_UNSIGNED_BYTE;
			case DataType::F16:   return GL_HALF_FLOAT;
			case DataType::I16:   return GL_SHORT;
			case DataType::U16:   return GL_UNSIGNED_SHORT;
			case DataType::I32:   return GL_INT;
			case DataType::U32:   return GL_UNSIGNED_INT;
			case DataType::F32:   return GL_FLOAT;
			case DataType::F64:   return GL_DOUBLE;
			// case DataType::F32X2: return GL_FLOAT_MAT2;
			// case DataType::F32X3: return GL_FLOAT_MAT3;
			// case DataType::F32X4: return GL_FLOAT_MAT4;
		}

	}

	uint32_t texture_format_to_gl_internal(TextureFormat format)
	{
		switch(format)
		{
			case TextureFormat::R16: return GL_R16;
			case TextureFormat::R16F: return GL_R16F;
			case TextureFormat::RG16F: return GL_RG16F;
			case TextureFormat::RG32F: return GL_RG32F;
			case TextureFormat::RG8: return GL_RG8;
			case TextureFormat::RGB8: return GL_RGB8;
			case TextureFormat::RGBA8: return GL_RGBA8;
			case TextureFormat::RGB16F: return GL_RGB16F;
			case TextureFormat::RGB32F: return GL_RGB32F;
			case TextureFormat::SRGB8: return GL_SRGB8;
			case TextureFormat::SRGB8A8: return GL_SRGB8_ALPHA8;
			case TextureFormat::DEPTH16: return GL_DEPTH_COMPONENT16;
			case TextureFormat::DEPTH24: return GL_DEPTH_COMPONENT24;
			case TextureFormat::DEPTH32: return GL_DEPTH_COMPONENT32;
			case TextureFormat::DEPTH32F: return GL_DEPTH_COMPONENT32F;
		}
	}

	DataType texture_format_to_data_type(TextureFormat format)
	{
		switch(format)
		{
			case TextureFormat::R16: return DataType::U16;
			case TextureFormat::R16F: return DataType::F16;
			case TextureFormat::RG16F: return DataType::F16;
			case TextureFormat::RG32F: return DataType::F32;
			case TextureFormat::RG8: return DataType::U8;
			case TextureFormat::RGB8: return DataType::U8;
			case TextureFormat::RGBA8: return DataType::U8;
			case TextureFormat::RGB16F: return DataType::F32;
			case TextureFormat::RGB32F: return DataType::F32;
			case TextureFormat::SRGB8: return DataType::U8;
			case TextureFormat::SRGB8A8: return DataType::U8;
			case TextureFormat::DEPTH16: return DataType::F16;
			case TextureFormat::DEPTH24: return DataType::F16; // DON't KNOW
			case TextureFormat::DEPTH32: return DataType::F32;
			case TextureFormat::DEPTH32F: return DataType::F32;
		}
	}

	uint32_t texture_format_to_gl_base(TextureFormat format)
	{
		switch(format)
		{
			case TextureFormat::R16: return GL_RED;
			case TextureFormat::R16F: return GL_RED;
			case TextureFormat::RG16F: return GL_RG;
			case TextureFormat::RG32F: return GL_RG;
			case TextureFormat::RG8: return GL_RG;
			case TextureFormat::RGB8: return GL_RGB;
			case TextureFormat::RGBA8: return GL_RGBA;
			case TextureFormat::RGB16F: return GL_RGB;
			case TextureFormat::RGB32F: return GL_RGB;
			case TextureFormat::SRGB8: return GL_RGB;
			case TextureFormat::SRGB8A8: return GL_RGBA;
			case TextureFormat::DEPTH16: return GL_DEPTH_COMPONENT;
			case TextureFormat::DEPTH24: return GL_DEPTH_COMPONENT;
			case TextureFormat::DEPTH32: return GL_DEPTH_COMPONENT;
			case TextureFormat::DEPTH32F: return GL_DEPTH_COMPONENT;
		}
	}

	DataType gl_data_type_to_data_type(uint32_t gl_type)
	{
		switch (gl_type)
		{
			case GL_BYTE: return DataType::I8;
			case GL_UNSIGNED_BYTE: return DataType::U8;
			case GL_SHORT: return DataType::I16;
			case GL_UNSIGNED_SHORT: return DataType::U16;
			case GL_INT: return DataType::I32;
			case GL_UNSIGNED_INT: return DataType::U32;
			case GL_HALF_FLOAT: return DataType::F16;
			case GL_FLOAT: return DataType::F32;
			case GL_FLOAT_VEC2:
			case GL_FLOAT_VEC3:
			case GL_FLOAT_VEC4: return DataType::F32;
			case GL_INT_VEC2:
			case GL_INT_VEC3:
			case GL_INT_VEC4: return DataType::I32;
			case GL_BOOL: return DataType::B8;
			case GL_BOOL_VEC2:
			case GL_BOOL_VEC3:
			case GL_BOOL_VEC4: return DataType::B8;
			case GL_FLOAT_MAT2:
			case GL_FLOAT_MAT3:
			case GL_FLOAT_MAT4: return DataType::F32;
		}
	}

	uint32_t texture_format_to_gl_data_type(TextureFormat format)
	{
		return data_type_to_gl(texture_format_to_data_type(format));
	}
}
