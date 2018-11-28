#include "render.h"

void Render::AddTriangle(BaseSceneContext &context, Vertex2D v0, Vertex2D v1, Vertex2D v2)
{
	Renderable2D triangle;
	triangle.type = Renderable2D::TRIANGLE;
	triangle.vertices[0] = v0;
	triangle.vertices[1] = v1;
	triangle.vertices[2] = v2;

	
	// TODO(Connor): Revaluate this back culling correcting code
	#ifdef TRIANGLE_WINDING_CCW
		b8 should_swap = !IsTriangleWindingCW(v0.position, v1.position, v2.position);
	#else
		b8 should_swap = IsTriangleWindingCW(v0.position, v1.position, v2.position);
	#endif

	if(should_swap) swap(triangle.vertices[1], triangle.vertices[2]);

	context.render_queue.push_back(triangle);
}

void Render::DrawAllInQueue(BaseSceneContext &context)
{
	for(u32 i = 0; i < context.render_queue.size(); ++i)
	{
		Renderable2D& current = context.render_queue[i];

		if(current.type == Renderable2D::TRIANGLE)
		{
			Render::DrawTriangleToBuffer(context.buffer, 
								  	(Vec3i){current.vertices[0].position.x, current.vertices[0].position.y, current.vertices[0].depth},
									(Vec3i){current.vertices[1].position.x, current.vertices[1].position.y, current.vertices[1].depth},
									(Vec3i){current.vertices[2].position.x, current.vertices[2].position.y, current.vertices[2].depth},
									current.vertices[0].colour, current.vertices[1].colour, current.vertices[2].colour);
		}else{
			assert(false);
		}
	}

	context.render_queue.clear();
}