#include "src\maths\maths.h"
#include "src\utils\timer.h"

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
	//ofstream
	//end ofstream
	Window window("Hiraeth", 1600, 900);

	mat4 ortho = mat4::orthographic(0.0f, 16.0f, 0.0f, 9.0f, -1.0f, 1.0f);

	std::vector<Tile> tiles;
	Shader* s = new Shader("src/shaders/map.vert", "src/shaders/map.frag");
	Shader& shader = *s;

	Camera camera(&window);
#define SAVE 0
#define MAP 1
#if SAVE
#if MAP
	Map map("map4.png", tiles, &window, &camera);
	map.serialize_tiles();
#else
	for (int y = -350; y < 0; y += 59)
		for (int x = -1000; x < 600; x += 89)
			tiles.push_back(Tile(vec2(x, y), 1.0f, maths::vec2(1 + (rand() % 6)*91, 268), maths::vec2(90, 59), 0));
	Map map("map4.png", tiles, &window, &camera);
	map.serialize_tiles();
#endif
#else
	#if MAP
	Map map("map4.png", 2, &window, &camera);
#else
	Map map("map4.png", 1, &window, &camera);
#endif
#endif
//#if SAVE
//	std::ofstream file("my_file.save");
//		cereal::BinaryOutputArchive oarchive(file); // Create an output archive
//
//		std::unique_ptr<Tile const> const m1{
//			new Tile(vec2(-90, 0), 1.0f, vec2(1, 268), vec2(90, 59), 0) };
//		oarchive(m1);
//#else
//	std::ifstream file("my_file.save");
//		cereal::BinaryInputArchive iarchive(file);
//
//		std::unique_ptr<Tile> deserialized_message{ nullptr };
//		iarchive(deserialized_message);
//		Tile m1(*deserialized_message.get());
//		tiles.push_back(m1);
//#endif


	Timer time;
	float timer = 0;

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