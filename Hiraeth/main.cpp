#include "utils\timer.h"
#include "src\maths\maths.h"

#include "src\graphics\window.h"
#include "keyboard\keyboard.h"
#include "view\camera.h"

#include "src\graphics\shader.h"
#include "src\map\map.h"
#include "graphics\texturemanager.h"

#include <time.h>
#include <cereal\archives\binary.hpp>
#include <fstream>
#include <cereal\types\memory.hpp>
#include <cereal\types\vector.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

int main()
{
	using namespace hiraeth;
	using namespace maths;
	using namespace graphics;
	using namespace map;
	using namespace view;

	//FT_Library ft;
	//if (FT_Init_FreeType(&ft)) {
	//	fprintf(stderr, "FATAL: Could not init FreeType");
	//	return 1;
	//}
	////Init Arial FreeType Face
	//FT_Face arial;
	//if (FT_New_Face(ft, "arial", 0, &arial))
	//{
	//	fprintf(stderr, "FATAL: Could not init font \"Arial\"");
	//	return 1;
	//}

	input::Keyboard keyboard;
	Window window("Hiraeth", 1600, 900, &keyboard);

	mat4 ortho = mat4::Orthographic(0.0f, 16.0f, 0.0f, 9.0f, -1.0f, 1.0f);

	std::vector<Tile> tiles;

	Timer time;
	float timer = 0;

	Camera camera(&window, &time);


	Map map("map4.png", 2, &window, &camera, &time);

	unsigned int frames = 0;
	while (!window.closed())
	{
		window.clear();
		double x, y;
		window.getKeyboard()->getMousePosition(x, y);
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