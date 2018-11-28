/*
	============================================
	Project:	Abacus Engine
	File: 		render_backend.h
	Author: 	Connor Macdonald
	--------------------------------------------
	Provides the low level functions which allow
	for software rendering to the graphics 
	context. This will become a more outdated
	file as the project moves towards hardware
	acceleration. For now it exists as an 
	experiment to test out these lower level 
	procedures.
	============================================
*/

#ifndef RENDER_BACKEND_H
#define RENDER_BACKEND_H
#include "atomics.h"
#include "math_helper.h"
#include <iostream>


struct GenericBuffer
{
	s8*			data;
	u16			width;
	u16 		height;
};

struct Colour
{
	// TODO(Connor): Replace with faster initlisation 
	Colour(u8 _r = 0, u8 _g = 0, u8 _b = 0, u8 _z = 0)
	{
		//TODO(Connor): Replace with os specifics
		r = _r;
		g = _g;
		b = _b;
		_null = _z;
	};

	union
	{
		s32 data_word = 0;

		// Colour's data types can change order for os spec
		struct
		{
		// TODO(Connor): Revaluate if this needs to be os dependent
		#ifdef OS_LINUX
			u8 b;
			u8 g;
			u8 r;

			// Allows for aliases
			union
			{
				u8 _null;
				u8 z;
				u8 a;
			};
		#endif
		};
	};
	
};

namespace Render
{
	// TODO(Connor): Replace with vec2 structs
	// OS Specfic
	inline void WritePixelToBuffer(const GenericBuffer &context, const u16 x, const u16 y, const Colour colour);
	inline void WritePixelToBuffer_i(const GenericBuffer &context, const u32 index, const Colour colour);

	// Respect Z ordering
	inline void WritePixelToBuffer_z(const GenericBuffer &context, const u16 x, const u16 y, const Colour colour);

	void ClearBuffer(GenericBuffer &context);
	void FillBuffer(GenericBuffer &context, Colour colour, u32 start, u32 end);
	
	// TODO(Connor): Parrellize these functions
	// Generic
	void DrawRectToBuffer(GenericBuffer &context, u16 pos_x, u16 pos_y, u16 width, u16 height, Colour colour);
	
	// TODO(Connor): Convert to a general vertex format
	void DrawTriangleToBuffer(GenericBuffer &context, Vec2i t0, Vec2i t1, Vec2i t2, Colour colour);
	void DrawTriangleToBuffer(GenericBuffer &context, Vec2i t0, Vec2i t1, Vec2i t2, Colour c0, Colour c1, Colour c2);

	// With Z indexs
	void DrawTriangleToBuffer(GenericBuffer &context, Vec3i t0, Vec3i t1, Vec3i t2, Colour colour);
	void DrawTriangleToBuffer(GenericBuffer &context, Vec3i t0, Vec3i t1, Vec3i t2, Colour c0, Colour c1, Colour c2);

}
#endif