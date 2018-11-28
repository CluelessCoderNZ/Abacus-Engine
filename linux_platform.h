/*
	============================================
	Project:	Abacus Engine
	File: 		linux_platform.h
	Author: 	Connor Macdonald
	--------------------------------------------
	This provides the entry point to the engine
	for the linux platform. Deploying graphics
	contexts which are then made generic.
	============================================
*/


#include <X11/Xlib.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <time.h> 
#include "atomics.h"
#include "render.h"

using namespace std;

#define MICROSECONDS_PER_SEC 1000000

struct LinuxImageBuffer
{
	XImage  *image = NULL;
	GenericBuffer generic;
};

struct LinuxInputContext
{
	Vec2i local_mouse;
	Vec2i global_mouse;
};

struct LinuxWindowContext
{
	Display *display;
	Window   main_window;
	Visual  *visual_context;

	u16		 win_width  = 768;
	u16		 win_height = 768;
	u32		 frame_time	= MICROSECONDS_PER_SEC/60;

	LinuxImageBuffer screen_buffer;
	LinuxInputContext input;

	Render::BaseSceneContext render_scene;
};

// Public known functions
s32 StartPlatformEngine();