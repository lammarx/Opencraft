#pragma once

#include "Window.h"

typedef unsigned int uint;

class Events
{
public:
	static bool* _keys;
	static uint* _frames;
	static uint _current;
	static float dx;
	static float dy;
	static float x;
	static float y;
	static bool _cursor_locked;
	static bool _cursor_started;

	static int init();
	static void pullEvents();

	static bool pressed(int keycode);
	static bool justPressed(int keycode);

	static bool clicked(int button);
	static bool justClicked(int button);
private:

};

