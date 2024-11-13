#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"

#define GL_BYTE 0x1400
#define GL_UNSIGNED_BYTE 0x1401
#define GL_SHORT 0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT 0x1404
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406
#define GL_DOUBLE 0x140A
#define GL_HALF_FLOAT 0x140B

namespace Noor
{
	uint32_t get_tinygltf_type_size(uint32_t type)
	{
		switch (type)
		{
			case GL_UNSIGNED_BYTE:
			case GL_BYTE:		    return 1;
			case GL_SHORT:
			case GL_UNSIGNED_SHORT:	    return 2;
			case GL_INT:
			case GL_UNSIGNED_INT:		    return 4;
			case GL_HALF_FLOAT:	return 2;
			case GL_FLOAT:	    return 4;
			case GL_DOUBLE: return 8;
		}
	}
}
