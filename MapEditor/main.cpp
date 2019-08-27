#include <iostream>
#include "graphics/buffers/buffer.h"
#include "graphics/window.h"
#include "input/keyboard.h"

int main() 
{
	hiraeth::input::Keyboard keyboard; 
	hiraeth::graphics::Window window("Hiraeth", 1600, 900, &keyboard);
	while (!window.closed())
	{
		window.clear();
		window.update();
	}
    return 0;
}