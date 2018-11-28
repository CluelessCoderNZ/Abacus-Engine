#include "render_backend.h"




/*
	OS SPECFIC PIXEL BUFFER WRITES
*/
// NOTE(Connor): Each OS can write into this define for faster os specfic preformance
#ifdef OS_LINUX
	inline void Render::WritePixelToBuffer(const GenericBuffer &context, const u16 x, const u16 y, const Colour colour)
	{
		assert(x < context.width && y < context.height);
		s32* data_pointer = (s32*)(context.data + context.width*y*4 + x*4);
		*data_pointer = colour.data_word;
	}

	// Set buffer pixel by index
	inline void Render::WritePixelToBuffer_i(const GenericBuffer &context, const u32 index, const Colour colour)
	{
		assert(index < context.width*context.height);
		s32* data_pointer = (s32*)(context.data + index*4);
		*data_pointer = colour.data_word;
	}

	inline void Render::WritePixelToBuffer_z(const GenericBuffer &context, const u16 x, const u16 y, const Colour colour)
	{
		assert(x < context.width && y < context.height);
		s32* data_pointer = (s32*)(context.data + context.width*y*4 + x*4);
		
		// Only write if z buffer is greater
		if(colour.z >= (*((Colour*)data_pointer)).z)
		{
			*data_pointer = colour.data_word;
		}
	}


	void Render::ClearBuffer(GenericBuffer &context)
	{
		memset(context.data, 0, context.width*context.height*4);
	}

	void Render::FillBuffer(GenericBuffer &context, Colour colour, u32 start = 0, u32 end = 0)
	{
		// If End unset set to maximum
		if (end == 0) end = context.width * context.height;

		//TODO(Connor): Replace with pointer arithmitic
		s32* data_pointer;
		for(u32 i = start; i < end; ++i)
		{
			data_pointer = (s32*)(context.data + i*4);
			*data_pointer = colour.data_word;
		}
	}
#endif

/*
	GEOMETRIC FUNCTIONS
*/
void Render::DrawRectToBuffer(GenericBuffer &context, u16 pos_x, u16 pos_y, u16 width, u16 height, Colour colour)
{
	u16 max_y = clamp((u16)(pos_y + height), (u16)0, context.height);
	u16 max_x = clamp((u16)(pos_x + width), (u16)0, context.width);
	for(u16 y = pos_y; y < max_y; ++y)
	{
		for(u16 x = pos_x; x < max_x; ++x)
		{
			WritePixelToBuffer(context, x, y, colour);
		}
	}
}

void Render::DrawTriangleToBuffer(GenericBuffer &context, Vec2i t0, Vec2i t1, Vec2i t2, Colour colour)
{
	// NOTE(Connor): Make sure winding order is correct or else does not render

	// TODO(Connor): Revaluate wether this check needs to be here or up the pipeline
	#ifdef TRIANGLE_WINDING_CCW
		if(!IsTriangleWindingCW(t0, t1, t2)) return;
	#else
		if(IsTriangleWindingCW(t0, t1, t2)) return;
	#endif

	// Find pixel space
	u16 min_x = min(min(t0.x, t1.x), t2.x);
	u16 max_x = max(max(t0.x, t1.x), t2.x);
	u16 min_y = min(min(t0.y, t1.y), t2.y);
	u16 max_y = max(max(t0.y, t1.y), t2.y);

	// Render Triangle
	for(u16 y = min_y; y <= max_y; ++y)
	{
		for(u16 x = min_x; x <= max_x; ++x)
		{
			// Check if point in bounds lies within triangle
			Vec2i pixel_point = {x,y};
			if(TrianglePointIntersection(pixel_point, t0, t1, t2))
			{
				WritePixelToBuffer_z(context, x, y, colour);
			}
		}
	}
}

void Render::DrawTriangleToBuffer(GenericBuffer &context, Vec2i t0, Vec2i t1, Vec2i t2, Colour c0, Colour c1, Colour c2)
{
	// NOTE(Connor): Make sure winding order is correct or else does not render

	// TODO(Connor): Revaluate wether this check needs to be here or up the pipeline
	#ifdef TRIANGLE_WINDING_CCW
		if(!IsTriangleWindingCW(t0, t1, t2)) return;
	#else
		if(IsTriangleWindingCW(t0, t1, t2)) return;
	#endif

	// Find pixel space
	u16 min_x = min(min(t0.x, t1.x), t2.x);
	u16 max_x = max(max(t0.x, t1.x), t2.x);
	u16 min_y = min(min(t0.y, t1.y), t2.y);
	u16 max_y = max(max(t0.y, t1.y), t2.y);

	// Render Triangle
	for(u16 y = min_y; y <= max_y; ++y)
	{
		for(u16 x = min_x; x <= max_x; ++x)
		{
			// Check if point in bounds lies within triangle
			Vec2i pixel_point = {x,y};
			if(WithinRectBounds(pixel_point, {context.width, context.height}) && 
			   TrianglePointIntersection(pixel_point, t0, t1, t2))
			{
				Vec3f weights = GetBarycentricCoordinates(pixel_point, t0, t1, t2);

				Colour final_colour;
				final_colour.r = c0.r * weights.x + c1.r * weights.y + c2.r * weights.z;
				final_colour.g = c0.g * weights.x + c1.g * weights.y + c2.g * weights.z;
				final_colour.b = c0.b * weights.x + c1.b * weights.y + c2.b * weights.z;

				WritePixelToBuffer_z(context, x, y, final_colour);
			}
		}
	}
}

void Render::DrawTriangleToBuffer(GenericBuffer &context, Vec3i t0, Vec3i t1, Vec3i t2, Colour colour)
{
	// NOTE(Connor): Make sure winding order is correct or else does not render

	// TODO(Connor): Revaluate wether this check needs to be here or up the pipeline
	#ifdef TRIANGLE_WINDING_CCW
		if(!IsTriangleWindingCW(t0, t1, t2)) return;
	#else
		if(IsTriangleWindingCW(t0, t1, t2)) return;
	#endif

	// Find pixel space
	u16 min_x = min(min(t0.x, t1.x), t2.x);
	u16 max_x = max(max(t0.x, t1.x), t2.x);
	u16 min_y = min(min(t0.y, t1.y), t2.y);
	u16 max_y = max(max(t0.y, t1.y), t2.y);

	// Render Triangle
	for(u16 y = min_y; y <= max_y; ++y)
	{
		for(u16 x = min_x; x <= max_x; ++x)
		{
			// Check if point in bounds lies within triangle
			Vec2i pixel_point = {x,y};
			if(WithinRectBounds(pixel_point, {context.width, context.height}) && 
			   TrianglePointIntersection(pixel_point, {t0.x, t0.y}, {t1.x, t1.y}, {t2.x, t2.y}))
			{
				Vec3f weights = GetBarycentricCoordinates(pixel_point, {t0.x, t0.y}, {t1.x, t1.y}, {t2.x, t2.y});
				colour.z = t0.z * weights.x + t1.z * weights.y + t2.z * weights.z;
				WritePixelToBuffer_z(context, x, y, colour);
			}
		}
	}
}

void Render::DrawTriangleToBuffer(GenericBuffer &context, Vec3i t0, Vec3i t1, Vec3i t2, Colour c0, Colour c1, Colour c2)
{
	// NOTE(Connor): Make sure winding order is correct or else does not render

	// TODO(Connor): Revaluate wether this check needs to be here or up the pipeline
	#ifdef TRIANGLE_WINDING_CCW
		if(!IsTriangleWindingCW(t0, t1, t2)) return;
	#else
		if(IsTriangleWindingCW(t0, t1, t2)) return;
	#endif

	// Find pixel space
	s16 min_x = min(min(t0.x, t1.x), t2.x);
	s16 max_x = max(max(t0.x, t1.x), t2.x);
	s16 min_y = min(min(t0.y, t1.y), t2.y);
	s16 max_y = max(max(t0.y, t1.y), t2.y);

	// Render Triangle
	for(s16 y = min_y; y <= max_y; ++y)
	{
		for(s16 x = min_x; x <= max_x; ++x)
		{
			// Check if point in bounds lies within triangle
			Vec2i pixel_point = {x,y};

			s32 t0_area = DistanceFromEdge({t1.x, t1.y}, {t2.x, t2.y}, pixel_point);
			s32 t1_area = DistanceFromEdge({t2.x, t2.y}, {t0.x, t0.y}, pixel_point);
			s32 t2_area = DistanceFromEdge({t0.x, t0.y}, {t1.x, t1.y}, pixel_point);

			#ifdef TRIANGLE_WINDING_CCW
				b8 is_inside = (t0_area >= 0) && (t1_area >= 0) && (t2_area >= 0);
			#else
				b8 is_inside = (t0_area <= 0) && (t1_area <= 0) && (t2_area <= 0);
			#endif

			if(WithinRectBounds(pixel_point, {context.width, context.height}) && 
			   is_inside)
			{
				s32 triangle_area = DistanceFromEdge(t0, t1, t2);
				Vec3f weights;
				weights.x = ((r32)t0_area)/triangle_area;
				weights.y = ((r32)t1_area)/triangle_area;
				weights.z = ((r32)t2_area)/triangle_area;

				Colour final_colour;
				final_colour.r = c0.r * weights.x + c1.r * weights.y + c2.r * weights.z;
				final_colour.g = c0.g * weights.x + c1.g * weights.y + c2.g * weights.z;
				final_colour.b = c0.b * weights.x + c1.b * weights.y + c2.b * weights.z;
				final_colour.z = t0.z * weights.x + t1.z * weights.y + t2.z * weights.z;
				WritePixelToBuffer_z(context, x, y, final_colour);
			}
		}
	}
}