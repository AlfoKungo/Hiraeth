#include "utils\timer.h"
#include "src\maths\maths.h"

#include "src\graphics\window.h"
#include "view\camera.h"

#include "src\graphics\shader.h"
#include "src\map\map.h"
#include "View\Camera.h"

#include <time.h>
#include <cereal\archives\binary.hpp>
#include <fstream>
#include <cereal\types\memory.hpp>
#include <cereal\types\vector.hpp>


int main()
{
	using namespace hiraeth;
	using namespace maths;
	using namespace graphics;
	using namespace map;
	using namespace view;
	Window window("Hiraeth", 1600, 900);

	mat4 ortho = mat4::orthographic(0.0f, 16.0f, 0.0f, 9.0f, -1.0f, 1.0f);

	std::vector<Tile> tiles;

	Camera camera(&window);

	Timer time;
	float timer = 0;

	Map map("map4.png", 2, &window, &camera, &time);

	unsigned int frames = 0;
	while (!window.closed())
	{
		window.clear();
		double x, y;
		window.getMousePosition(x, y);
		camera.update();
		map.update();
		map.draw();


		window.update();
		frames++;
		if (time.elapsed() - timer > 1.0f)
		{
			timer += 1.0f;
			printf("%d fps\n", frames);
			frames = 0;
		}

	}
	return 0;
}