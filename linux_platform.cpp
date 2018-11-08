#include "linux_platform.h"

s32 start_platform_engine()
{
	// Get current display
	Display *display;
	display = XOpenDisplay(NULL);
	if (display == NULL) {
		// TODO(Connor): Replace with logging
      	cout << "Error: Could not open display" << endl;
      	return -1;
   	}

   	// Get default screen id
   	s32 screen_id;
	screen_id = DefaultScreen(display);

	// Create the engines main window
   	Window main_window;
   	main_window = XCreateSimpleWindow(display, 
   									  RootWindow(display, screen_id), 
   									  10, 10, 100, 100, 1,
	                       			  BlackPixel(display, screen_id), 
	                       			  WhitePixel(display, screen_id));

   	// Set windows event input
	XSelectInput(display, main_window, ExposureMask | KeyPressMask);
	XMapWindow(display, main_window);

	// Get Window managers message to close window
	Atom wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", false);

	// Tell X server to capture this message
	XSetWMProtocols(display, main_window, &wmDeleteMessage, 1);

	// Loop over event pump
	XEvent win_event;
	string msg = "Hello, World!";
	flag running = true;

	while (running)
	{
		// Check Event
		XNextEvent(display, &win_event);
		switch (win_event.type)
		{
			case Expose:
			{
				XFillRectangle(display, main_window, DefaultGC(display, screen_id), 20, 20, 10, 10);
		 		XDrawString(display, main_window, DefaultGC(display, screen_id), 10, 50, msg.c_str(), msg.length());
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
				running = false;
			} break;
		}
	}

	// Clean up
	XUnmapWindow(display, main_window);
	XDestroyWindow(display, main_window);
	XCloseDisplay(display);

	return 0;
}