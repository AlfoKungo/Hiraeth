#include <cereal/archives/xml.hpp>
#include <fstream>
#include <istream>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <iostream>
#include <filesystem>
#include <array>

#include "srl/map_data.h"
#include <cereal/archives/json.hpp>

#define USED_MAPS 2

using namespace SRL;

FootHoldData HorizontalFootHold(vec2 p1, float length)
{
	return FootHoldData{ p1 - vec2{ 22.5f, -3 }, vec2{ p1.x + length + 22.5f, p1.y + 3 }, vec2{ 0, 1 } };
}
FootHoldData HorizontalFootHold(vec2 p1, vec2 p2)
{
	return FootHoldData{ p1 - vec2{22.5f, -3}, p2 + vec2{22.5f, 3 }, vec2{ 0, 1 } };
}

FootHoldData VerticalFootHold(vec2 p1, float length)
{
	return FootHoldData{ p1, vec2{p1.x, p1.y + length}, vec2{1, 0} };
}
FootHoldData VerticalFootHold(vec2 p1, vec2 p2)
{
	return FootHoldData{ p1, p2, vec2{ 1, 0 } };
}

void serialize_map_data()
{
	MapData map_data[USED_MAPS]{ MapData{}, MapData{} };

	//Generate data
	for (int i = 0; i < 2; i++)
		map_data[0].Portals.push_back(PortalData{ vec2{ float(i * 200), 0 }, 1 });
	for (int y = -450; y < -100; y += 59)
		for (int x = -800; x < 800; x += 89)
			map_data[0].Tiles.push_back(TileData{ vec2{ float(x), float(y) }, 1.0f, unsigned int(rand() % 6) });
	map_data[0].Tiles.push_back(TileData{ vec2{ -90, 0 }, 1.0f, 0 });
	map_data[0].Tiles.push_back(TileData{ vec2{ 0, 0 }, 1.0f, 0 });
	map_data[0].Tiles.push_back(TileData{ vec2{ -190, 0 }, 1.0f, 0 });
	//map_data[0].Tiles.push_back(TileData{ vec2{ 0, 0 }, 1.0f, vec2{ 0, 0 }, vec2{ 621, 328 }, 0 });
	map_data[0].FootHolds.push_back(HorizontalFootHold(vec2{ -800, -100 }, vec2{ 800, -100 }));
	//map_data[0].m_FootHolds.push_back(HorizontalFootHold(vec2(-800, 0), vec2(800,-200)));
	map_data[0].FootHolds.push_back(HorizontalFootHold(vec2{ 800, -150 }, vec2{ 1000, -150 }));
	map_data[0].FootHolds.push_back(HorizontalFootHold(vec2{ 800, -50 }, vec2{ 1000, -50 }));
	map_data[0].FootHolds.push_back(VerticalFootHold(vec2{ 1000, -200 }, 600));
	map_data[0].FootHolds.push_back(VerticalFootHold(vec2{ -700, -200 }, 600));
	map_data[0].FootHolds.push_back(VerticalFootHold(vec2{ -800, -450 }, 350));
	map_data[0].FootHolds.push_back(VerticalFootHold(vec2{ 800, -450 }, 350));
	map_data[0].Summons.push_back(Summon{ 1, vec2{ -200, -90 } });
	map_data[0].Summons.push_back(Summon{ 2, vec2{ 200, -90 } });
	map_data[0].Npcs.push_back(0);

	for (int i = 0; i < 2; i++)
		map_data[1].Portals.push_back(PortalData{ vec2{ float((i * (-200)) - 200), 0 }, 0 });
	for (int y = -350; y < 0; y += 59)
		for (int x = -1000; x < 600; x += 89)
			//map_data[1].Tiles.push_back(TileData{ vec2{ float(x), float(y) }, 1.0f, vec2{ 1 + float((rand() % 6) * 91), 268 },{ 90, 59 }, 0 });
			map_data[1].Tiles.push_back(TileData{ vec2{ float(x), float(y) }, 1.0f, unsigned int(rand() % 6) });
	map_data[1].FootHolds.push_back(HorizontalFootHold(vec2{ -800, -100 }, vec2{ 800, -100 }));
	map_data[1].FootHolds.push_back(HorizontalFootHold(vec2{ 800, -150 }, vec2{ 1000, -150 }));
	map_data[1].FootHolds.push_back(HorizontalFootHold(vec2{ 800, -50 }, vec2{ 1000, -50 }));
	//map_data[1].m_FootHolds.push_back(FootHold(vec2(-800, 0), vec2(800,-200), vec2(0, 1)));
	map_data[1].FootHolds.push_back(VerticalFootHold(vec2{ 1000, -200 }, 600));
	map_data[1].FootHolds.push_back(VerticalFootHold(vec2{ -700, -200 }, 600));
	map_data[1].FootHolds.push_back(VerticalFootHold(vec2{ -800, -450 }, 350));
	map_data[1].FootHolds.push_back(VerticalFootHold(vec2{ 800, -450 }, 350));
	map_data[1].Summons.push_back(Summon{ 1, vec2{ 20, -90 } });
	map_data[1].Summons.push_back(Summon{ 2, vec2{ -20, -90 } });
	map_data[1].Npcs.push_back(0);

	//Serialize Data
	for (int i = 0; i < USED_MAPS; i++)
	{
		std::ofstream out("output/" + std::to_string(i) + ".xml", std::ios::out | std::ios::binary);
		cereal::XMLOutputArchive ar(out);
		ar(map_data[i]);
	}

	//Serialize Data
	for (int i = 0; i < USED_MAPS; i++)
	{
		std::ofstream out("output/" + std::to_string(i) + ".json", std::ios::out | std::ios::binary);
		cereal::JSONOutputArchive ar(out);
		ar(map_data[i]);
	}
}

int main()
{

	serialize_map_data();

	std::cout << "finished";
	//std::cin.get();
	return 0;
}