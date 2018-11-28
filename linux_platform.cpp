#include "linux_platform.h"

LinuxImageBuffer CreateBuffer(LinuxWindowContext &ctx, u32 width, u32 height)
{
	LinuxImageBuffer buffer;

	u32 data_size = width * height * 4;
	buffer.generic.data = (s8*)malloc(data_size);
	buffer.generic.width = width;
	buffer.generic.height = height;

	buffer.image = XCreateImage(ctx.display, ctx.visual_context, 24, ZPixmap, 0, (char*)buffer.generic.data, width, height, 32, 4*width);
	assert(buffer.image != NULL);

	return buffer;
}

s32 StartPlatformEngine()
{
	// Init context
	LinuxWindowContext ctx; 

	// Get current display
	ctx.display = XOpenDisplay(NULL);
	if (ctx.display == NULL) {
		// TODO(Connor): Replace with logging
      	cout << "Error: Could not open display" << endl;
      	return -1;
   	}

   	// Get default screen id
   	s32 screen_id;
	screen_id = DefaultScreen(ctx.display);
	
	// Get screens visual context
	ctx.visual_context = DefaultVisual(ctx.display, screen_id);

	// Create the engines main window
   	
   	ctx.main_window = XCreateSimpleWindow(ctx.display, 
   									  RootWindow(ctx.display, screen_id), 
   									  10, 10, ctx.win_width, ctx.win_height, 1,
	                       			  BlackPixel(ctx.display, screen_id), 
	                       			  WhitePixel(ctx.display, screen_id));

   	// Generate window buffers
   	ctx.screen_buffer = CreateBuffer(ctx, ctx.win_width, ctx.win_height);
   	ctx.render_scene.buffer = ctx.screen_buffer.generic;

   	// Set windows event input
	XSelectInput(ctx.display, ctx.main_window, ExposureMask | KeyPressMask);
	XMapWindow(ctx.display, ctx.main_window);
	XStoreName(ctx.display, ctx.main_window, "ABACUS");

	// Get Window managers message to close window
	Atom wmDeleteMessage = XInternAtom(ctx.display, "WM_DELETE_WINDOW", false);

	// Tell X server to capture this message
	XSetWMProtocols(ctx.display, ctx.main_window, &wmDeleteMessage, 1);

	// Loop over event pump
	flag running = true;

	clock_t time_frame_start;
	r32 	delta_t = 1.0;
	while (running)
	{
		time_frame_start = clock();

		// Check Events
		XEvent win_event;
		while(XPending(ctx.display) > 0)
		{
			XNextEvent(ctx.display, &win_event);
			switch (win_event.type)
			{
				case Expose:
				{
					// NOTE(Connor): Should do something I think
				} break;

				case ClientMessage:
				{
					// Check message is window managers delete message
					if (win_event.xclient.data.l[0] == wmDeleteMessage)
					{
	                	running = false;
					}
				} break;

				case KeyPress:
				{
					
				} break;
			}
		}
		// Update Mouse Input
		Window  return_window; 
		u32		pointer_state;
		XQueryPointer(ctx.display, ctx.main_window, &return_window, &return_window, &ctx.input.global_mouse.x, &ctx.input.global_mouse.y, &ctx.input.local_mouse.x, &ctx.input.local_mouse.y, &pointer_state);


		/*
			TEMPORARY CODE
		*/
		r32 cs = 0.1;
		Vertex3D cube_vertices[8]= {Vertex3D({cs,cs,cs}, Colour(255,255,255)),
									Vertex3D({cs,cs,-cs}, Colour(255,255,0)),
									Vertex3D({cs,-cs,cs}, Colour(255,0,255)),
									Vertex3D({cs,-cs,-cs}, Colour(255,0,0)),
									Vertex3D({-cs,cs,cs}, Colour(0,255,255)),
									Vertex3D({-cs,cs,-cs}, Colour(0,255,0)),
									Vertex3D({-cs,-cs,cs}, Colour(0,0,255)),
									Vertex3D({-cs,-cs,-cs}, Colour(50,50,50))};
		u8		 cube_indices[36] = {0,1,2, 1,3,2,
									 4,1,0, 4,5,1,
									 4,6,5, 6,7,5,
									 6,2,7, 7,2,3,
									 7,3,5, 5,3,1,
									 6,2,4, 4,2,0};
		
		// Setup Projection Matrix
		r32 fov = 80;
		r32 f = 3;
		r32 n = 0.5;
		Mat4 proj;
		r32 S = 1.0/tan((fov/2)*(PI/180));
		proj = {S, 0, 0, 0,
		        0, S, 0, 0,
		     	0, 0, -f/(f-n), -(f*n)/(f-n),
		        0, 0, -1, 0};

		// Setup Model Matrix
		static r32 a = 0;
		a += 0.05;

		Vec3f offset = {2*((r32)ctx.input.local_mouse.x / ctx.win_width - 0.5) , 2 * ((r32)ctx.input.local_mouse.y / ctx.win_height - 0.5), -2};
		Mat4 model;

		// Rotation
		model.data[0][0] = cos(a);
		model.data[0][2] = sin(a);
		model.data[2][0] = -sin(a);
		model.data[2][2] = cos(a);

		// Translation
		model.data[0][3] = offset.x;
		model.data[1][3] = offset.y;
		model.data[2][3] = offset.z;

		// Render cube triangles
		for(s32 index = 0; index < 36; index += 3)
		{
			Vertex3D v0 = cube_vertices[cube_indices[index    ]];
			Vertex3D v1 = cube_vertices[cube_indices[index + 1]];
			Vertex3D v2 = cube_vertices[cube_indices[index + 2]];

			// Transformation
			Vec4f p0 = proj * (model * (Vec4f){v0.position.x, v0.position.y, v0.position.z, 1});
			Vec4f p1 = proj * (model * (Vec4f){v1.position.x, v1.position.y, v1.position.z, 1});
			Vec4f p2 = proj * (model * (Vec4f){v2.position.x, v2.position.y, v2.position.z, 1});

			v0.position.x = p0.x / p0.w; v0.position.y = p0.y / p0.w; v0.position.z = p0.z / p0.w;
			v1.position.x = p1.x / p1.w; v1.position.y = p1.y / p1.w; v1.position.z = p1.z / p1.w;
			v2.position.x = p2.x / p2.w; v2.position.y = p2.y / p2.w; v2.position.z = p2.z / p2.w;

			// After Transformation
			Vertex2D t0 = Vertex2D({(0.5 + v0.position.x)*ctx.win_width, (0.5+v0.position.y)*ctx.win_height}, v0.colour, 255 - 255*v0.position.z);
			Vertex2D t1 = Vertex2D({(0.5 + v1.position.x)*ctx.win_width, (0.5+v1.position.y)*ctx.win_height}, v1.colour, 255 - 255*v1.position.z);
			Vertex2D t2 = Vertex2D({(0.5 + v2.position.x)*ctx.win_width, (0.5+v2.position.y)*ctx.win_height}, v2.colour, 255 - 255*v2.position.z);

			Render::AddTriangle(ctx.render_scene, t0, t1, t2);
		}

		//Render::AddTriangle(ctx.render_scene,  Vertex2D({0,0}),  Vertex2D({ctx.input.local_mouse.x, ctx.input.local_mouse.y}),  Vertex2D({0,300}));

		/*
			END OF TEMPORARY
		*/


		// Render all in queue
		Render::DrawAllInQueue(ctx.render_scene);

		// Send Buffer to server
		XPutImage(ctx.display, ctx.main_window, DefaultGC(ctx.display, screen_id), ctx.screen_buffer.image, 0, 0, 0, 0, ctx.screen_buffer.image->width, ctx.screen_buffer.image->height);
		XSync(ctx.display, false);
		Render::ClearBuffer(ctx.screen_buffer.generic); 

		// Measure time to sleep
		clock_t processing_time = clock() - time_frame_start;
		u32 ms_processing_time = (((r32)processing_time) / CLOCKS_PER_SEC) * MICROSECONDS_PER_SEC;

		cout << "frame utilisation: " << (r32)ms_processing_time / ctx.frame_time << endl;

		assert(ms_processing_time > 0);
		if(ms_processing_time < ctx.frame_time)
		{
			// Sleep for remaing time of the frame
			s32 sleep_time = ctx.frame_time - ms_processing_time;
			usleep(sleep_time);
			delta_t = 1.0;
		}else{
			// Get new frames delta t for a preformance hitch
			clock_t end_frame_time = clock() - time_frame_start;
			delta_t = ((((r32)end_frame_time) / CLOCKS_PER_SEC) * MICROSECONDS_PER_SEC) / ctx.frame_time;
		}

	}

	// Clean up
	XUnmapWindow(ctx.display, ctx.main_window);
	XDestroyWindow(ctx.display, ctx.main_window);
	XCloseDisplay(ctx.display);

	return 0;
}