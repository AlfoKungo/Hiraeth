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
#include "code_for_checks/npc_data_check.h"
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
const int END_OF_FILE = -1;

const int MONSTER_AMOUNT = MAX_ADDRESSES;
const int MONSTER_TEXTURES_AMOUNT = MAX_ADDRESSES;
const int MONSTER_ADDRESSES_AMOUNT(MONSTER_AMOUNT + MONSTER_TEXTURES_AMOUNT);

const int MONSTER_DATA_ADDRESS_BEGIN = 0;
const int MONSTER_TEXTURES_ADDRESS_BEGIN = (MONSTER_AMOUNT * sizeof(Address));
const int MONSTER_DATA_DATA_BEGIN = (MONSTER_ADDRESSES_AMOUNT * sizeof(Address));

namespace fs = std::filesystem;



template<class DATA>
void serialize_vector(std::vector<DATA> data, int addresses_begin, std::ofstream& out, cereal::BinaryOutputArchive& arout)
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

template <class TData>
void dsrl_json(TData& Td, const std::string path)
{
	std::ifstream in(path, std::ios::in);
	cereal::JSONInputArchive arin(in);
	arin(Td);
}

//template <class TData, std::enable_if_t<TData::texture_data::type, SRL::TextureData>>
//template <class TData, typename = typename std::enable_if<true>::type>
template <class TData = SRL::UseItemData>
//void tex_n_data(std::string path, std::vector<TData>& data_vec)
//std::enable_if_t<std::is_same<TData, SRL::UseItemData>::value || std::is_same<TData, SRL::EquipItemData>::value>
//std::enable_if_t<std::is_same<typename TData::texture_data*, SRL::TextureData>::value>
//std::enable_if_t<std::is_same<TData::TextureData, SRL::TextureData>::value>::value, void>
void tex_n_data(std::string path, std::vector<TData>& data_vec)
 //tex_n_data(std::string path, typename std::enable_if<std::is_same<typename TData::texture_data, SRL::TextureData>, std::vector<TData>>::type & data_vec)
{
	TData Td{};

	const std::string tex_path = path + "\\tex.png";
	dsrl_json(Td.info, path + "\\data.json");

	Td.texture_data.load_texture(tex_path);
	data_vec.push_back(Td);
}

template <class TData = SRL::UseItemData>
void just_tex(std::string path, std::vector<TData>& data_vec)
{
	TData Td{};
	Td.load_texture(path);
	data_vec.push_back(Td);
}

template <class TData = SRL::EquipItemData>
void equip_data(std::string path, std::vector<TData> & data_vec)
{
	TData Td{};

	const std::string tex_path = path + "\\tex.png";
	dsrl_json(Td.info, path + "\\data.json");

	for (auto& pp : fs::directory_iterator(path))
	{
		if (pp.path().filename().string() == "default.default.png")
		{
			//SRL::FullAnimationData fad;
			//fad.load_data(pp.path().parent_path().string(), "default.default");
			//Td.textures[SRL::as_Default] = fad;
			Td.textures[SRL::as_Default].tex_data.load_texture(path + "\\default.default.png");
			dsrl_json(Td.textures[SRL::as_Default].org, (path + "\\default.default.json"));
		}
	}
	
	Td.icon_texture.load_texture(tex_path);
	data_vec.push_back(Td);
}

template <class TData = SRL::SkillData>
//typename std::enable_if<
//	std::conditional<std::is_same<TData, SRL::UseItemData>::value || std::is_same<TData, SRL::EquipItemData>::value, 
//std::false_type, std::true_type>::value>::type
void  skill_data(std::string path, std::vector<TData>& data_vec)
{
	TData Td{};

	//Td.skill_info = dsrl_json<Td.skill_info>(Td.skill_info, path + "\\data.json");
	dsrl_json(Td.skill_info, path + "\\data.json");

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
	TData Td{};
	dsrl_json(Td, path);
	data_vec.push_back(Td);
}

template <class TData>
 void monster_data(std::string path, std::vector<TData>& data_vec)
{
				SRL::MonsterTexturesData Mtd{};
				Mtd.creature_sprites.hit_box = { 45,50 };

				Mtd.creature_sprites.sprited_data[SRL::MoveState::Stand].load_data(path, "stand");
				Mtd.creature_sprites.sprited_data[SRL::MoveState::Walk].load_data(path, "walk");
				Mtd.creature_sprites.sprited_data[SRL::MoveState::Hit].load_data(path, "hit");
				Mtd.creature_sprites.sprited_data[SRL::MoveState::Die].load_data(path, "die");
				data_vec.push_back(Mtd);
}

//template <class TData, std::enable_if_t<std::conditional<std::is_same<SRL::TextureData, TData::texture_data>::value, std::true_type, std::false_type>>>
//template <class TData, std::enable_if_t<std::is_same<SRL::UseItemData, TData>::value> = 0>
//template <class TData, typename std::enable_if<std::is_same<SRL::UseItemData, TData>::value>::type = 0>
//template <class TData, typename std::enable_if<std::true_type> = 0>
//typename std::enable_if<std::is_same<TData, SRL::UseItemData>::value || std::is_same<TData, SRL::EquipItemData>::value, void>::type
template <class TData, class F>
void serialize_generic(const std::string SRL_TYPE, F fun)
{
	serialize_generic<TData>(SRL_TYPE, fun, DATA_BEGIN, ADDRESS_BEGIN);
}
template <class TData, class F>
void serialize_generic(const std::string SRL_PATH, F fun, int DataBegin, int AddressBegin)
{
	std::string srl_type = SRL_PATH;;
	if (SRL_PATH.find('/') > 0)
		srl_type = SRL_PATH.substr(0, SRL_PATH.find("/"));
	const std::string path = "../Hiraeth/serialized/" + srl_type + ".data";
	if (!fs::exists(path))
	{
		std::ofstream outfile(path);
		outfile.close();
	}
	std::ofstream data_file(path,
		std::ios::out | std::ios::in | std::ios::binary);
	if (data_file.is_open())
	{
		cereal::BinaryOutputArchive arout(data_file);
		std::vector<TData> data_vec; //
		data_vec.reserve(100);

		for (auto & p : fs::directory_iterator("data/" + SRL_PATH))
		{
			fun(p.path().string(), data_vec);
		}

		if (DataBegin == -1)
			data_file.seekp(0, std::ostream::end);
		else
			data_file.seekp(DataBegin);
		serialize_vector(data_vec, AddressBegin, data_file, arout);
	}
}

bool copyFile(const char* SRC, const char* DEST)
{
	std::ifstream src(SRC, std::ios::binary);
	std::ofstream dest(DEST, std::ios::binary);
	dest << src.rdbuf();
	return src && dest;
}


int main()
{
	const std::filesystem::path pathToDelete("../Hiraeth/serialized");
	remove_all(pathToDelete);
	create_directories(pathToDelete);

	// Serialize Map Data
	serialize_generic<SRL::MapData>("map/map_data", just_data<SRL::MapData>,
		MAP_DATA_DATA_BEGIN, MAP_DATA_ADDRESSES_BEGIN);
	serialize_generic<SRL::TextureData>("map/textures", just_tex<SRL::TextureData>,
		END_OF_FILE, TEXTURES_ADDRESSES_BEGIN);
	serialize_generic<SRL::TileTextureData>("map/tiles", tex_n_data<SRL::TileTextureData>,
		END_OF_FILE, TILES_ADDRESSES_BEGIN);

	// Serialize Monster Data
	SRL::Checks::create_monster_data();
	serialize_generic<SRL::MonsterData>("monster/monster_data", just_data<SRL::MonsterData>,
		MONSTER_DATA_DATA_BEGIN, MONSTER_DATA_ADDRESS_BEGIN);
	serialize_generic<SRL::MonsterTexturesData>("monster/textures", monster_data<SRL::MonsterTexturesData>,
		END_OF_FILE, MONSTER_TEXTURES_ADDRESS_BEGIN);

	// Serialize Item Data
	Checks::create_item_data();
	serialize_generic<SRL::UseItemData>("item", tex_n_data<SRL::UseItemData>);

	// Serialize Equip Data
	Checks::create_equip_data();
	serialize_generic<SRL::EquipItemData>("equip", equip_data<SRL::EquipItemData>);

	// Serialize NPC Data
	SRL::Checks::create_npc_data();
	serialize_generic<SRL::NpcData>("npc", tex_n_data<SRL::NpcData>);

	// Serialize Skills Data
	Checks::create_skill_data();
	serialize_generic<SRL::SkillData>("skills", skill_data<SRL::SkillData>);

	// Serialize Quest Data
	Checks::create_quest_data();
	serialize_generic<SRL::QuestData>("quest", just_data<SRL::QuestData>);

	std::string path_from = "../Hiraeth/serialized/";
	fs::path server_path = "../Server/serialized/";
	fs::path debug_path = "../x64/debug/serialized/";
	//fs::path targetParent = "../Server/serialized/";
	for (const auto& entry : fs::directory_iterator(path_from))
	{
		fs::path sourceFile = entry.path();
		auto target = server_path / sourceFile.filename(); // sourceFile.filename() returns "sourceFile.ext".
		fs::copy_file(sourceFile, target, fs::copy_options::overwrite_existing);
	}
	for (const auto& entry : fs::directory_iterator(path_from))
	{
		fs::path sourceFile = entry.path();
		auto target = debug_path / sourceFile.filename(); // sourceFile.filename() returns "sourceFile.ext".
		fs::copy_file(sourceFile, target, fs::copy_options::overwrite_existing);
	}

	std::cout << "finished";
	//std::cin.get();
	return 0;
}

