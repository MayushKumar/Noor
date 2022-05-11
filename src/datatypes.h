#pragma once

#include <cinttypes>

namespace Noor
{
	
	using DataType = uint32_t;
	
#define NOOR_BYTE		0x1400
#define NOOR_UBYTE		0x1401
#define NOOR_SHORT		0x1402
#define NOOR_USHORT		0x1403
#define NOOR_INT		0x1404
#define NOOR_UINT		0x1405
#define NOOR_FLOAT		0x1406
#define NOOR_FLOAT2	    0x8B50
#define NOOR_FLOAT3	    0x8B51
#define NOOR_FLOAT4	    0x8B52
#define NOOR_INT2	    0x8B53
#define NOOR_INT3	    0x8B54
#define NOOR_INT4	    0x8B55
#define NOOR_BOOL		0x8B56
#define NOOR_BOOL2	    0x8B57
#define NOOR_BOOL3	    0x8B58
#define NOOR_BOOL4	    0x8B59
// #define NOOR_FLOATMAT2	0x8B5A
// #define NOOR_FLOATMAT3	0x8B5B
// #define NOOR_FLOATMAT4	0x8B5C

	inline uint32_t data_type_size(DataType type)
	{
		switch (type)
		{
		case NOOR_BYTE:		return 1;
		case NOOR_UBYTE:	return 1;
		case NOOR_SHORT:	return 2;
		case NOOR_USHORT:	return 2;
		case NOOR_INT:		return 4;
		case NOOR_UINT:		return 4;
		case NOOR_FLOAT:	return 4;
		case NOOR_FLOAT2:	return 8;
		case NOOR_FLOAT3:	return 12;
		case NOOR_FLOAT4:	return 16;
		case NOOR_INT2:		return 8;
		case NOOR_INT3:		return 12;
		case NOOR_INT4:		return 16;
		case NOOR_BOOL:		return 1;
		case NOOR_BOOL2:	return 2;
		case NOOR_BOOL3:	return 3;
		case NOOR_BOOL4:	return 4;
		default:			return 0;
		}
	}
	
}    
