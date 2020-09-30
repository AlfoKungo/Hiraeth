#pragma once
#include <tuple>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include  <iterator>



namespace hiraeth {
	namespace network {
		enum DropType
		{
			DropType_multiple,
			DropType_only_one_etc,
		};
		struct MoneyRangeStruct
		{
			unsigned int min;
			unsigned int range;
		};
		struct DropStruct
		{
			DropType drop_type;
			std::tuple<std::vector<unsigned int>, float> inv_drops;
			std::tuple<std::vector<unsigned int>, float> use_drops;
			std::tuple<std::vector<unsigned int>, float> etc_drops;
			MoneyRangeStruct money_range;
			float percent;
		};
		static std::map<unsigned int, DropStruct> DropsPerMob{
		{0, {DropType_multiple, {{0,1,2}, 0.1f}, {{0,1,2}, 0.5f},{{0,1,2}, 0.5f}, {10, 10}, 0.5f} },
		{1, {DropType_multiple, {{0,1,2}, 0.1f}, {{0,1,2}, 0.5f},{{0,1,2}, 0.5f}, {10, 10}, 0.5f}},
		{2, {DropType_only_one_etc, {{{}}, 0.0f}, {{{}}, 0.0f},{{0}, 1.0f}, {10, 10}, 0.5f}},
		{3, {DropType_only_one_etc, {{{}}, 0.0f}, {{{}}, 0.0f},{{0}, 1.0f}, {10, 10}, 0.5f}},
		};
		static std::vector<unsigned int> select_randomly_from_vector(const std::vector<unsigned int>& in)
		{
			if (in.size() == 0)
				return {};

			std::vector<unsigned int> out;
			size_t nelems = 1;
			std::sample(
				in.begin(),
				in.end(),
				std::back_inserter(out),
				nelems,
				std::mt19937{ std::random_device{}() }
			);
			return out;
		}
		inline std::tuple<std::vector<unsigned int>, std::vector<unsigned int>, std::vector<unsigned int>, unsigned int >
			calculate_drops(unsigned int mob_type_id)
		{
			auto drop_data = DropsPerMob[mob_type_id];
			if (drop_data.drop_type == DropType_multiple)
			{
				std::vector<unsigned int> items;
				unsigned int money_drop = drop_data.money_range.min + rand() % drop_data.money_range.range;
				return std::make_tuple(select_randomly_from_vector(std::get<0>(drop_data.inv_drops)),
					select_randomly_from_vector(std::get<0>(drop_data.inv_drops)),
					select_randomly_from_vector(std::get<0>(drop_data.inv_drops)), money_drop);
			}
			else
				if (drop_data.drop_type == DropType_only_one_etc)
				{
					//return make_tuple(std::vector<unsigned int> {}, std::vector<unsigned int> {}, std::get<0>(drop_data.etc_drops), 0);
					return make_tuple(std::vector<unsigned int> {}, std::get<0>(drop_data.etc_drops), std::vector<unsigned int> {}, 0);
				}
			return make_tuple(std::vector<unsigned int> {}, std::vector<unsigned int> {}, std::vector<unsigned int> {}, 0);
		}
		struct RandomStruct
		{
			SRL::EquipItemDataType stat_type;
			unsigned int min, max;
		};
		static std::map<unsigned int, std::vector<RandomStruct>> RandomStatsForEquips{
		{0, {{SRL::AttackPower, 10, 20}, {SRL::StrInc, 7,10} }},
		{1, {{SRL::AttackPower, 10, 20}, {SRL::StrInc, 7,10} }},
		{2, {{SRL::AttackPower, 10, 20}, {SRL::StrInc, 7,10} }},
		{3, {{SRL::AttackPower, 10, 20}, {SRL::StrInc, 7,10} }},
		{4, {{SRL::AttackPower, 10, 20}, {SRL::StrInc, 7,10} }}
		};
		static SRL::EquipPropertiesMap RandomizeEquipStats(unsigned int equip_type_id)
		{
			SRL::EquipPropertiesMap props{};
			for (auto [stat_type, min, max] : RandomStatsForEquips[equip_type_id])
			{
				auto range = max - min;
				auto val = min + rand() % range;
				props.emplace(stat_type, val);
			}
			return props;
		}

		static void ReduceEquipsData(std::map<SRL::EquipItemType, unsigned int> & reduced, const decltype(PlayerData::equips_char) & player_equips)
		{
			for (const auto& [equip_type, equip_data] : player_equips)
				reduced[equip_type] = equip_data.equip_id;
		}

	}
}
