#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <fstream>
#include <istream>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <iostream>
#include <filesystem>
#include <array>
#include "code_for_checks/monster_data_check.h"
#include <string>

#include "srl/ImageLoad.h"
#include "srl/map_data.h"
#include "srl/tile_texture_data.h"
#include "srl/monster_data.h"
#include "srl/item_data.h"
#include "srl/npc_data.h"
#include <cereal/archives/json.hpp>
#include "srl/skill_data.h"
#include "srl/animation_data.h"
#include "code_for_checks/skill_data_check.h"
#include "code_for_checks/item_data_check.h"
#include "code_for_checks/equip_data_check.h"
#include "code_for_checks/quest_data_check.h"

typedef int Address;

const int MAX_ADDRESSES = 100;

const int MAP_AMOUNT = MAX_ADDRESSES;
const int TEXTURES_AMOUNT = MAX_ADDRESSES;
const int TILES_AMOUNT = MAX_ADDRESSES;
const int MAP_ADDRESSES_AMOUNT = (TEXTURES_AMOUNT + MAP_AMOUNT + TILES_AMOUNT);

const int MAP_DATA_ADDRESSES_BEGIN = 0;
const int TEXTURES_ADDRESSES_BEGIN = (MAP_AMOUNT * sizeof(Address));
const int TILES_ADDRESSES_BEGIN = ((MAP_AMOUNT + TEXTURES_AMOUNT) * sizeof(Address));
const int MAP_DATA_DATA_BEGIN = (MAP_ADDRESSES_AMOUNT * sizeof(Address));

const int ADDRESS_BEGIN = 0;
const int DATA_BEGIN = MAX_ADDRESSES * sizeof(Address);

const int MONSTER_AMOUNT = MAX_ADDRESSES;
const int MONSTER_TEXTURES_AMOUNT = MAX_ADDRESSES;
const int MONSTER_ADDRESSES_AMOUNT(MONSTER_AMOUNT + MONSTER_TEXTURES_AMOUNT);

#define MONSTER_DATA_ADDRESS_BEGIN 0
#define MONSTER_TEXTURES_ADDRESS_BEGIN (MONSTER_AMOUNT * sizeof(Address))
#define MONSTER_DATA_DATA_BEGIN (MONSTER_ADDRESSES_AMOUNT * sizeof(Address))

#define ITEM_AMOUNT MAX_ADDRESSES
#define ITEM_ADDRESSES_AMOUNT (ITEM_AMOUNT)
#define ITEM_ADDRESS_BEGIN 0
#define ITEM_DATA_BEGIN (ITEM_ADDRESSES_AMOUNT * sizeof(Address))

#define NPC_AMOUNT MAX_ADDRESSES
#define NPC_ADDRESSES_AMOUNT (ITEM_AMOUNT)
#define NPC_ADDRESS_BEGIN 0
#define NPC_DATA_BEGIN (ITEM_ADDRESSES_AMOUNT * sizeof(Address))

#define SKILL_AMOUNT MAX_ADDRESSES
#define SKILL_ADDRESSES_AMOUNT (ITEM_AMOUNT)
#define SKILL_ADDRESS_BEGIN 0
#define SKILL_DATA_BEGIN (ITEM_ADDRESSES_AMOUNT * sizeof(Address))

#define USED_MAPS 2

namespace fs = std::experimental::filesystem;



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
//template <class TData, std::enable_if_t<TData::texture_data::type, SRL::TextureData>>
//template <class TData, typename = typename std::enable_if<true>::type>
template <class TData>
//void tex_n_data(std::string path, std::vector<TData>& data_vec)
//std::enable_if_t<std::is_same<TData, SRL::UseItemData>::value || std::is_same<TData, SRL::EquipItemData>::value>
//std::enable_if_t<std::is_same<typename TData::texture_data*, SRL::TextureData>::value>
//std::enable_if_t<std::is_same<TData::TextureData, SRL::TextureData>::value>::value, void>
void tex_n_data(std::string path, std::vector<TData>& data_vec)
 //tex_n_data(std::string path, typename std::enable_if<std::is_same<typename TData::texture_data, SRL::TextureData>, std::vector<TData>>::type & data_vec)
{
	const std::string tex_path = path + "\\tex.png";
	const std::string data_path = path + "\\data.json";
	std::ifstream in(data_path, std::ios::in);
	cereal::JSONInputArchive arin(in);
	TData Td{};

	arin(Td.info);
	Td.texture_data.load_texture(tex_path);
	data_vec.push_back(Td);
}

template <class TData>
//typename std::enable_if<
//	std::conditional<std::is_same<TData, SRL::UseItemData>::value || std::is_same<TData, SRL::EquipItemData>::value, 
//std::false_type, std::true_type>::value>::type
void  skill_data(std::string path, std::vector<TData>& data_vec)
{
	const std::string data_path = path + "\\data.json";
	std::ifstream in(data_path, std::ios::in);
	cereal::JSONInputArchive arin(in);
	TData Td{};

	arin(Td.skill_info);
	Td.icon_data.load_data(path);
	for (auto & pp : fs::directory_iterator(path))
	{
		if (pp.path().filename().string() == "effect.png")
		{
			SRL::FullAnimationData fad;
			fad.load_data(pp.path().parent_path().string(), "effect");
			Td.animation_map[SRL::effectAnimation] = fad;
		}
		if (pp.path().filename().string() == "ball.png")
		{
			SRL::FullAnimationData fad;
			fad.load_data(pp.path().parent_path().string(), "ball");
			Td.animation_map[SRL::ballAnimation] = fad;
		}
		if (pp.path().filename().string() == "hit.png")
		{
			SRL::FullAnimationData fad;
			fad.load_data(pp.path().parent_path().string(), "hit");
			Td.animation_map[SRL::hitAnimation] = fad;
		}
	}
	data_vec.push_back(Td);
}

template <class TData>
 void just_data(std::string path, std::vector<TData>& data_vec)
{
	const std::string data_path = path;
	std::ifstream in(data_path, std::ios::in);
	cereal::JSONInputArchive arin(in);
	TData Td{};

	arin(Td);
	data_vec.push_back(Td);
}

//template <class TData, std::enable_if_t<std::conditional<std::is_same<SRL::TextureData, TData::texture_data>::value, std::true_type, std::false_type>>>
//template <class TData, std::enable_if_t<std::is_same<SRL::UseItemData, TData>::value> = 0>
//template <class TData, typename std::enable_if<std::is_same<SRL::UseItemData, TData>::value>::type = 0>
//template <class TData, typename std::enable_if<std::true_type> = 0>
//typename std::enable_if<std::is_same<TData, SRL::UseItemData>::value || std::is_same<TData, SRL::EquipItemData>::value, void>::type
template <class TData, class F>
void serialize_generic(const std::string SRL_TYPE, F fun)
{
		std::ofstream data_file("../Hiraeth/serialized/" + SRL_TYPE + ".data", 
			std::ios::out | std::ios::binary);
		if (data_file.is_open())
		{
			cereal::BinaryOutputArchive arout(data_file);
			std::vector<TData> data_vec; //
			data_vec.reserve(100);

			for (auto & p : fs::directory_iterator("data/" + SRL_TYPE))
			{
				fun(p.path().string(), data_vec);
				//tex_n_data(p.path().string(), data_vec);
			}

			data_file.seekp(DATA_BEGIN);
			serialize_data(data_vec, ADDRESS_BEGIN, data_file, arout);
		}
}

int main()
{
		
	// Serialize Map Data
	{
		std::ofstream map_data_file("../Hiraeth/serialized/map.data", std::ios::out | std::ios::binary);
		if (map_data_file.is_open())
		{
			cereal::BinaryOutputArchive arout(map_data_file);

			std::vector<SRL::MapData> v_mapData;
			v_mapData.reserve(100);
			for (auto & p : fs::directory_iterator("data/map/map_data"))
			{
				std::ifstream in(p.path(), std::ios::in);
				cereal::JSONInputArchive arin(in);
				SRL::MapData map_data{};
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
				std::string tex_path = path + "\\tex.png";
				std::string data_path = path + "\\data.xml";
				std::ifstream in(data_path, std::ios::in);
				cereal::XMLInputArchive arin(in);
				SRL::TileTextureData Td{};

				arin(Td.TilesUV);
				Td.texture_data.load_texture(tex_path);
				v_tileTextureData.push_back(Td);
			}
			map_data_file.seekp(0, map_data_file.end);
			serialize_data(v_tileTextureData, TILES_ADDRESSES_BEGIN, map_data_file, arout);


			map_data_file.close();
		}
	}

	// Serialize Monster Data
	//Checks::create_monster_data();
	{
		std::ofstream monster_data_file("../Hiraeth/serialized/monster.data", std::ios::out | std::ios::binary);
		if (monster_data_file.is_open())
		{
			cereal::BinaryOutputArchive arout(monster_data_file);

			std::vector<SRL::MonsterData> v_monsterData;
			v_monsterData.reserve(100);
			for (auto & p : fs::directory_iterator("data/monster/monster_data"))
			{
				std::ifstream in(p.path(), std::ios::in);
				cereal::JSONInputArchive arin(in);
				SRL::MonsterData monster_data{};
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
				//std::ifstream in(path + "\\data.json", std::ios::in);
				//cereal::JSONInputArchive arin(in);
				//arin(Mtd.creature_sprites);

				Mtd.creature_sprites.hit_box = { 45,50 };

				Mtd.creature_sprites.sprited_data[SRL::MoveState::Stand].load_data(path, "stand");
				Mtd.creature_sprites.sprited_data[SRL::MoveState::Walk].load_data(path, "walk");
				Mtd.creature_sprites.sprited_data[SRL::MoveState::Hit].load_data(path, "hit");
				Mtd.creature_sprites.sprited_data[SRL::MoveState::Die].load_data(path, "die");
				//Mtd.textures_dict[SRL::MoveState::Walk].load_texture(path + "\\walk.png");
				//Mtd.textures_dict[SRL::MoveState::Stand].load_texture(path + "\\stand.png");
				//Mtd.textures_dict[SRL::MoveState::Hit].load_texture(path + "\\hit.png");
				v_monsterTexturesData.push_back(Mtd);
			}
			monster_data_file.seekp(0, std::ofstream::end);
			serialize_data(v_monsterTexturesData, MONSTER_TEXTURES_ADDRESS_BEGIN, monster_data_file, arout);
		}
	}

	// Serialize Item Data
	Checks::create_item_data();
	{
		serialize_generic<SRL::UseItemData>("item", tex_n_data<SRL::UseItemData>);
/*
		std::ofstream item_data_file("../Hiraeth/serialized/item.data", std::ios::out | std::ios::binary);
		if (item_data_file.is_open())
		{
			cereal::BinaryOutputArchive arout(item_data_file);
			std::vector<SRL::UseItemData> v_itemData;
			v_itemData.reserve(100);

			for (auto & p : fs::directory_iterator("data/item"))
			{
				std::string path = p.path().string();
				std::string data_path = path + "\\data.json";
				std::string tex_path = path + "\\tex.png";
				std::ifstream in(data_path, std::ios::in);
				cereal::JSONInputArchive arin(in);
				SRL::UseItemData Td{};

				arin(Td.item_info);
				Td.texture_data.load_texture(tex_path);
				v_itemData.push_back(Td);
			}
			item_data_file.seekp(ITEM_DATA_BEGIN);
			serialize_data(v_itemData, ITEM_ADDRESS_BEGIN, item_data_file, arout);
		}
		*/
	}

	// Serialize Equip Data
	Checks::create_equip_data();
	{
		serialize_generic<SRL::EquipItemData>("equip", tex_n_data<SRL::EquipItemData>);
/*
		std::ofstream item_data_file("../Hiraeth/serialized/equip.data", std::ios::out | std::ios::binary);
		if (item_data_file.is_open())
		{
			cereal::BinaryOutputArchive arout(item_data_file);
			std::vector<SRL::EquipItemData> v_itemData;
			v_itemData.reserve(100);

			for (auto & p : fs::directory_iterator("data/equip"))
			{
				std::string path = p.path().string();
				std::string data_path = path + "\\data.json";
				std::string tex_path = path + "\\tex.png";
				std::ifstream in(data_path, std::ios::in);
				cereal::JSONInputArchive arin(in);
				SRL::EquipItemData Td{};

				arin(Td.item_info);
				Td.texture_data.load_texture(tex_path);
				v_itemData.push_back(Td);
			}
			item_data_file.seekp(ITEM_DATA_BEGIN);
			serialize_data(v_itemData, ITEM_ADDRESS_BEGIN, item_data_file, arout);
		}
		*/
	}

	// Serialize NPC Data
	{
		serialize_generic<SRL::NpcData>("npc",tex_n_data<SRL::NpcData>);
		//const std::string SRL_TYPE = "npc";
		//std::ofstream npc_data_file("../Hiraeth/serialized/" + SRL_TYPE + ".data", 
		//	std::ios::out | std::ios::binary);
		//if (npc_data_file.is_open())
		//{
		//	cereal::BinaryOutputArchive arout(npc_data_file);

		//	std::vector<SRL::NpcData> v_npcData;
		//	v_npcData.reserve(100);
		//	for (auto & p : fs::directory_iterator("data/" + SRL_TYPE))
		//	{
		//		std::string path = p.path().string();
		//		std::string data_path = path + "\\data.json";
		//		std::string tex_path = path + "\\tex.png";
		////		std::string tex_path = path + "\\stand.0.png";
		//		std::ifstream in(data_path, std::ios::in);
		//		cereal::JSONInputArchive arin(in);
		//		SRL::NpcData Td{};

		//		arin(Td.info);
		//		Td.texture_data.load_texture(tex_path);
		//		v_npcData.push_back(Td);
		//	}
		//	npc_data_file.seekp(NPC_DATA_BEGIN);
		//	serialize_data(v_npcData, NPC_ADDRESS_BEGIN, npc_data_file, arout);
		//}
	}

	// Serialize Skills Data
	Checks::create_skill_data();
	{
		serialize_generic<SRL::SkillData>("skills",skill_data<SRL::SkillData>);
		//const std::string SRL_TYPE = "skills";
		//std::ofstream skill_data_file("../Hiraeth/serialized/" + SRL_TYPE + ".data", 
		//	std::ios::out | std::ios::binary);
		//if (skill_data_file.is_open())
		//{
		//	cereal::BinaryOutputArchive arout(skill_data_file);

		//	std::vector<SRL::SkillData> skill_data;
		//	skill_data.reserve(100);
		//	for (auto & p : fs::directory_iterator("data/" + SRL_TYPE))
		//	{
		//		std::string path = p.path().string();
		//		std::string data_path = path + "\\data.json";
		//		std::ifstream in(data_path, std::ios::in);
		//		cereal::JSONInputArchive arin(in);
		//		SRL::SkillData Td{};

		//		arin(Td.skill_info);
		//		Td.icon_data.load_data(path);
		//		for (auto & pp : fs::directory_iterator(p))
		//		{
		//			if (pp.path().filename().string() == "effect.png")
		//			{
		//				SRL::FullAnimationData fad;
		//				fad.load_data(pp.path().parent_path().string(), "effect");
		//				Td.animation_map[SRL::effectAnimation] = fad;
		//			}
		//			if (pp.path().filename().string() == "ball.png")
		//			{
		//				SRL::FullAnimationData fad;
		//				fad.load_data(pp.path().parent_path().string(), "ball");
		//				Td.animation_map[SRL::ballAnimation] = fad;
		//			}
		//			if (pp.path().filename().string() == "hit.png")
		//			{
		//				SRL::FullAnimationData fad;
		//				fad.load_data(pp.path().parent_path().string(), "hit");
		//				Td.animation_map[SRL::hitAnimation] = fad;
		//			}
		//		}
		//		skill_data.push_back(Td);
		//	}
		//	skill_data_file.seekp(SKILL_DATA_BEGIN);
		//	serialize_data(skill_data, SKILL_ADDRESS_BEGIN, skill_data_file, arout);
		//}
	}

	// Serialize Skills Data
	Checks::create_quest_data();
	serialize_generic<SRL::QuestData>("quest",just_data<SRL::QuestData>);
	//{
	//	const std::string SRL_TYPE = "quest"; //
	//	std::ofstream skill_data_file("../Hiraeth/serialized/" + SRL_TYPE + ".data", 
	//		std::ios::out | std::ios::binary);
	//	if (skill_data_file.is_open())
	//	{
	//		cereal::BinaryOutputArchive arout(skill_data_file);

	//		std::vector<SRL::SkillData> skill_data; //
	//		skill_data.reserve(100);
	//		for (auto & p : fs::directory_iterator("data/" + SRL_TYPE))
	//		{
	//			std::string path = p.path().string();
	//			std::string data_path = path + "\\data.json";
	//			std::ifstream in(data_path, std::ios::in);
	//			cereal::JSONInputArchive arin(in);
	//			SRL::SkillData Td{};

	//			arin(Td.skill_info);
	//			Td.icon_data.load_data(path);
	//			for (auto & pp : fs::directory_iterator(p))
	//			{
	//				if (pp.path().filename().string() == "effect.png")
	//				{
	//					SRL::FullAnimationData fad;
	//					fad.load_data(pp.path().parent_path().string(), "effect");
	//					Td.animation_map[SRL::effectAnimation] = fad;
	//				}
	//				if (pp.path().filename().string() == "ball.png")
	//				{
	//					SRL::FullAnimationData fad;
	//					fad.load_data(pp.path().parent_path().string(), "ball");
	//					Td.animation_map[SRL::ballAnimation] = fad;
	//				}
	//				if (pp.path().filename().string() == "hit.png")
	//				{
	//					SRL::FullAnimationData fad;
	//					fad.load_data(pp.path().parent_path().string(), "hit");
	//					Td.animation_map[SRL::hitAnimation] = fad;
	//				}
	//			}
	//			skill_data.push_back(Td);
	//		}
	//		skill_data_file.seekp(SKILL_DATA_BEGIN);
	//		serialize_data(skill_data, SKILL_ADDRESS_BEGIN, skill_data_file, arout);
	//	}
	//}


	std::cout << "finished serializing " << "map" << std::endl;
	std::cout << "finished";
	//std::cin.get();
	return 0;
}

