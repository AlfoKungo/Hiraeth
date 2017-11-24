#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <fstream>
#include <istream>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <iostream>
#include <filesystem>
#include <array>
#include <string>

#include "srl/ImageLoad.h"
#include "srl/map_data.h"
#include "srl/tile_texture_data.h"
#include "srl/monster_data.h"

#define MAX_ADDRESSES 100

#define MAP_AMOUNT MAX_ADDRESSES
#define TEXTURES_AMOUNT MAX_ADDRESSES
#define TILES_AMOUNT MAX_ADDRESSES
#define MAP_ADDRESSES_AMOUNT (TEXTURES_AMOUNT + MAP_AMOUNT + TILES_AMOUNT)

#define MAP_DATA_ADDRESSES_BEGIN 0
#define TEXTURES_ADDRESSES_BEGIN (MAP_AMOUNT * sizeof(Address))
#define TILES_ADDRESSES_BEGIN ((MAP_AMOUNT + TEXTURES_AMOUNT) * sizeof(Address))
#define MAP_DATA_DATA_BEGIN (MAP_ADDRESSES_AMOUNT * sizeof(Address))


#define MONSTER_AMOUNT MAX_ADDRESSES
#define MONSTER_TEXTURES_AMOUNT MAX_ADDRESSES
#define MONSTER_ADDRESSES_AMOUNT (MONSTER_AMOUNT + MONSTER_TEXTURES_AMOUNT)

#define MONSTER_DATA_ADDRESS_BEGIN 0
#define MONSTER_TEXTURES_ADDRESS_BEGIN (MONSTER_AMOUNT * sizeof(Address))
#define MONSTER_DATA_DATA_BEGIN (MONSTER_ADDRESSES_AMOUNT * sizeof(Address))

#define USED_MAPS 2

namespace fs = std::experimental::filesystem;

const int zero = 0;

typedef int Address;


template<class DATA>
void serialize_data(std::vector<DATA> data, int addresses_begin, std::ofstream& out, cereal::BinaryOutputArchive& arout)
{

	int i = 0;
	Address addresses[MAX_ADDRESSES];
	std::fill_n(addresses, MAP_AMOUNT, 0);
	std::cout << "start serializing data at address " << out.tellp() << std::endl;
	for (auto & d : data)
	{
		addresses[i] = out.tellp();
		arout(d);
		i++;
	}
	out.seekp(addresses_begin);
	std::cout << "start serializing addresses at address " << out.tellp() << std::endl;
	for (const Address& address : addresses)
		arout(address);
}


int main()
{

	// Serialize Map Data
	{
		std::ofstream map_data_file("output/map.data", std::ios::out | std::ios::binary);
		if (map_data_file.is_open())
		{
			cereal::BinaryOutputArchive arout(map_data_file);

			std::vector<SRL::MapData> v_mapData;
			v_mapData.reserve(100);
			for (auto & p : fs::directory_iterator("data/map/map_data"))
			{
				std::ifstream in(p.path(), std::ios::in);
				cereal::XMLInputArchive arin(in);
				SRL::MapData map_data = SRL::MapData{};
				arin(map_data);
				v_mapData.push_back(map_data);
			}
			map_data_file.seekp(MAP_DATA_DATA_BEGIN);
			serialize_data(v_mapData, MAP_DATA_ADDRESSES_BEGIN, map_data_file, arout);

			std::vector<SRL::TextureData> v_textureData;
			v_textureData.reserve(100);
			for (auto & p : fs::directory_iterator("data/map/textures"))
			{
				SRL::TextureData texture_data{};
				texture_data.load_texture(p.path().string());
				v_textureData.push_back(texture_data);
			}
			map_data_file.seekp(0, map_data_file.end);
			serialize_data(v_textureData, TEXTURES_ADDRESSES_BEGIN, map_data_file, arout);

			std::vector<SRL::TileTextureData> v_tileTextureData;
			v_tileTextureData.reserve(100);
			for (auto & p : fs::directory_iterator("data/map/tiles"))
			{
				std::string path = p.path().string();
				std::string tile_name = path + "\\tile.png";
				std::string tile_data_name = path + "\\tile_data.xml";
				std::ifstream in(tile_data_name, std::ios::in);
				cereal::XMLInputArchive arin(in);
				SRL::TileTextureData Td{};

				arin(Td.TilesUV);
				Td.texture_data.load_texture(tile_name);
				v_tileTextureData.push_back(Td);
			}
			map_data_file.seekp(0, map_data_file.end);
			serialize_data(v_tileTextureData, TILES_ADDRESSES_BEGIN, map_data_file, arout);


			map_data_file.close();
		}
	}

	// Serialize Monster Data
	{
		std::ofstream monster_data_file("output/monster.data", std::ios::out | std::ios::binary);
		if (monster_data_file.is_open())
		{
			cereal::BinaryOutputArchive arout(monster_data_file);

			std::vector<SRL::MonsterData> v_monsterData;
			v_monsterData.reserve(100);
			for (auto & p : fs::directory_iterator("data/monster/monster_data"))
			{
				std::ifstream in(p.path(), std::ios::in);
				cereal::XMLInputArchive arin(in);
				SRL::MonsterData monster_data = SRL::MonsterData{};
				arin(monster_data);
				v_monsterData.push_back(monster_data);
			}
			monster_data_file.seekp(MONSTER_DATA_DATA_BEGIN);
			serialize_data(v_monsterData, MONSTER_DATA_ADDRESS_BEGIN, monster_data_file, arout);

			std::vector<SRL::MonsterTexturesData> v_monsterTexturesData;
			v_monsterTexturesData.reserve(100);
			for (auto & p : fs::directory_iterator("data/monster/textures"))
			{
				SRL::MonsterTexturesData Mtd{};

				std::string path = p.path().string();
				std::ifstream in(path + "\\data.xml", std::ios::in);
				cereal::XMLInputArchive arin(in);

				arin(Mtd.frames_amount);
				Mtd.walk_texture.load_texture(path + "\\walk.png");
				Mtd.stand_texture.load_texture(path + "\\stand.png");
				Mtd.hit_texture.load_texture(path + "\\hit.png");
				v_monsterTexturesData.push_back(Mtd);
			}
			monster_data_file.seekp(0, monster_data_file.end);
			serialize_data(v_monsterTexturesData, MONSTER_TEXTURES_ADDRESS_BEGIN, monster_data_file, arout);
		}
	}

	std::cout << "finished serializing " << "map" << std::endl;
	std::cout << "finished";
	std::cin.get();
	return 0;
}