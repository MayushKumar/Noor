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

#define NOOR_RG8 0x822B
#define NOOR_RGB8 0x8051
#define NOOR_RGBA8 0x8058

#define NOOR_NEAREST 0x2600
#define NOOR_LINEAR 0x2601
#define NOOR_NEAREST_MIPMAP_NEAREST 0x2700
#define NOOR_LINEAR_MIPMAP_NEAREST 0x2701
#define NOOR_NEAREST_MIPMAP_LINEAR 0x2702
#define NOOR_LINEAR_MIPMAP_LINEAR 0x2703
#define NOOR_REPEAT 0x2901

/* THESE KEYS ARE EXACT COPIES OF GLFW KEYS */

#define NOOR_RELEASED                0
#define NOOR_PRESSED                 1
#define NOOR_REPEATED                2

#define NOOR_KEY_SPACE              32
#define NOOR_KEY_APOSTROPHE         39  /* ' */
#define NOOR_KEY_COMMA              44  /* , */
#define NOOR_KEY_MINUS              45  /* - */
#define NOOR_KEY_PERIOD             46  /* . */
#define NOOR_KEY_SLASH              47  /* / */
#define NOOR_KEY_0                  48
#define NOOR_KEY_1                  49
#define NOOR_KEY_2                  50
#define NOOR_KEY_3                  51
#define NOOR_KEY_4                  52
#define NOOR_KEY_5                  53
#define NOOR_KEY_6                  54
#define NOOR_KEY_7                  55
#define NOOR_KEY_8                  56
#define NOOR_KEY_9                  57
#define NOOR_KEY_SEMICOLON          59  /* ; */
#define NOOR_KEY_EQUAL              61  /* = */
#define NOOR_KEY_A                  65
#define NOOR_KEY_B                  66
#define NOOR_KEY_C                  67
#define NOOR_KEY_D                  68
#define NOOR_KEY_E                  69
#define NOOR_KEY_F                  70
#define NOOR_KEY_G                  71
#define NOOR_KEY_H                  72
#define NOOR_KEY_I                  73
#define NOOR_KEY_J                  74
#define NOOR_KEY_K                  75
#define NOOR_KEY_L                  76
#define NOOR_KEY_M                  77
#define NOOR_KEY_N                  78
#define NOOR_KEY_O                  79
#define NOOR_KEY_P                  80
#define NOOR_KEY_Q                  81
#define NOOR_KEY_R                  82
#define NOOR_KEY_S                  83
#define NOOR_KEY_T                  84
#define NOOR_KEY_U                  85
#define NOOR_KEY_V                  86
#define NOOR_KEY_W                  87
#define NOOR_KEY_X                  88
#define NOOR_KEY_Y                  89
#define NOOR_KEY_Z                  90
#define NOOR_KEY_LEFT_BRACKET       91  /* [ */
#define NOOR_KEY_BACKSLASH          92  /* \ */
#define NOOR_KEY_RIGHT_BRACKET      93  /* ] */
#define NOOR_KEY_GRAVE_ACCENT       96  /* ` */
#define NOOR_KEY_WORLD_1            161 /* non-US #1 */
#define NOOR_KEY_WORLD_2            162 /* non-US #2 */

#define NOOR_KEY_ESCAPE             256
#define NOOR_KEY_ENTER              257
#define NOOR_KEY_TAB                258
#define NOOR_KEY_BACKSPACE          259
#define NOOR_KEY_INSERT             260
#define NOOR_KEY_DELETE             261
#define NOOR_KEY_RIGHT              262
#define NOOR_KEY_LEFT               263
#define NOOR_KEY_DOWN               264
#define NOOR_KEY_UP                 265
#define NOOR_KEY_PAGE_UP            266
#define NOOR_KEY_PAGE_DOWN          267
#define NOOR_KEY_HOME               268
#define NOOR_KEY_END                269
#define NOOR_KEY_CAPS_LOCK          280
#define NOOR_KEY_SCROLL_LOCK        281
#define NOOR_KEY_NUM_LOCK           282
#define NOOR_KEY_PRINT_SCREEN       283
#define NOOR_KEY_PAUSE              284
#define NOOR_KEY_F1                 290
#define NOOR_KEY_F2                 291
#define NOOR_KEY_F3                 292
#define NOOR_KEY_F4                 293
#define NOOR_KEY_F5                 294
#define NOOR_KEY_F6                 295
#define NOOR_KEY_F7                 296
#define NOOR_KEY_F8                 297
#define NOOR_KEY_F9                 298
#define NOOR_KEY_F10                299
#define NOOR_KEY_F11                300
#define NOOR_KEY_F12                301
#define NOOR_KEY_F13                302
#define NOOR_KEY_F14                303
#define NOOR_KEY_F15                304
#define NOOR_KEY_F16                305
#define NOOR_KEY_F17                306
#define NOOR_KEY_F18                307
#define NOOR_KEY_F19                308
#define NOOR_KEY_F20                309
#define NOOR_KEY_F21                310
#define NOOR_KEY_F22                311
#define NOOR_KEY_F23                312
#define NOOR_KEY_F24                313
#define NOOR_KEY_F25                314
#define NOOR_KEY_KP_0               320
#define NOOR_KEY_KP_1               321
#define NOOR_KEY_KP_2               322
#define NOOR_KEY_KP_3               323
#define NOOR_KEY_KP_4               324
#define NOOR_KEY_KP_5               325
#define NOOR_KEY_KP_6               326
#define NOOR_KEY_KP_7               327
#define NOOR_KEY_KP_8               328
#define NOOR_KEY_KP_9               329
#define NOOR_KEY_KP_DECIMAL         330
#define NOOR_KEY_KP_DIVIDE          331
#define NOOR_KEY_KP_MULTIPLY        332
#define NOOR_KEY_KP_SUBTRACT        333
#define NOOR_KEY_KP_ADD             334
#define NOOR_KEY_KP_ENTER           335
#define NOOR_KEY_KP_EQUAL           336
#define NOOR_KEY_LEFT_SHIFT         340
#define NOOR_KEY_LEFT_CONTROL       341
#define NOOR_KEY_LEFT_ALT           342
#define NOOR_KEY_LEFT_SUPER         343
#define NOOR_KEY_RIGHT_SHIFT        344
#define NOOR_KEY_RIGHT_CONTROL      345
#define NOOR_KEY_RIGHT_ALT          346
#define NOOR_KEY_RIGHT_SUPER        347
#define NOOR_KEY_MENU               348

#define NOOR_MOUSE_BUTTON_1         0
#define NOOR_MOUSE_BUTTON_2         1
#define NOOR_MOUSE_BUTTON_3         2
#define NOOR_MOUSE_BUTTON_4         3
#define NOOR_MOUSE_BUTTON_5         4
#define NOOR_MOUSE_BUTTON_6         5
#define NOOR_MOUSE_BUTTON_7         6
#define NOOR_MOUSE_BUTTON_8         7
#define NOOR_MOUSE_BUTTON_LAST      NOOR_MOUSE_BUTTON_8
#define NOOR_MOUSE_BUTTON_LEFT      NOOR_MOUSE_BUTTON_1
#define NOOR_MOUSE_BUTTON_RIGHT     NOOR_MOUSE_BUTTON_2
#define NOOR_MOUSE_BUTTON_MIDDLE    NOOR_MOUSE_BUTTON_3

#define NOOR_MOD_SHIFT           0x0001
#define NOOR_MOD_CONTROL         0x0002
#define NOOR_MOD_ALT             0x0004
#define NOOR_MOD_SUPER           0x0008
#define NOOR_MOD_CAPS_LOCK       0x0010
#define NOOR_MOD_NUM_LOCK        0x0020
	
}    
