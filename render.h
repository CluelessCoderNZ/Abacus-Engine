/*
	============================================
	Project:	Abacus Engine
	File: 		render.h
	Author: 	Connor Macdonald
	--------------------------------------------
	Provides higher level abstraction of 
	rendering this file wraps render functions 
	for software or hardware accelrated 
	rendering aswell as providing some datatypes
	for delayed order rendering.
	============================================
*/

#ifndef RENDER_H
#define RENDER_H
#include "render_backend.h"
#include <vector>

// TODO(Connor):Add Generic Template
struct Vertex2D
{
	Vertex2D(Vec2i _position = {0,0}, Colour _colour = Colour(255,255,255), u8 _depth = 0)
	{
		position = _position;
		colour = _colour;
		depth = _depth;
	}
	Vec2i  position;
	Colour colour;
	u8     depth;
};

// TODO(Connor): Delete since not used internally only externally with TEMPOARY code
struct Vertex3D
{
	Vertex3D(Vec3f _position = {0,0,0}, Colour _colour = Colour(255,255,255))
	{
		position = _position;
		colour = _colour;
	}
	Vec3f  position;
	Colour colour;
};

namespace Render
{
	struct Renderable2D
	{
		enum Type
		{
			TRIANGLE
		};

		Type   type = TRIANGLE;
		Vertex2D vertices[3];
	};

	struct BaseSceneContext
	{
		// TODO(Connor): Replace with own allocator
		std::vector<Renderable2D> render_queue;

		GenericBuffer buffer;
	};

	void AddTriangle(BaseSceneContext &context, Vertex2D v0, Vertex2D v1, Vertex2D v2);
	void DrawAllInQueue(BaseSceneContext &context);
}


#endif