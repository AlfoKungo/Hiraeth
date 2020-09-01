#pragma once
#include "cereal/types/variant.hpp"
#include "cereal/types/map.hpp"
#include "cereal/types/tuple.hpp"
#include "cereal/types/vector.hpp"
#include "vector"
#include "cereal/cereal.hpp"
#include "cereal/types/concepts/pair_associative_container.hpp"
#include "equip_item_data.h"

namespace SRL {

	enum QuestTab {
		Available = 0,
		InProgress = 1,
		Done = 2,
		Recommended = 3,
	};

	struct QuestDBStruct
	{
		unsigned int type{};
		unsigned int stage{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(type), CEREAL_NVP(stage));
		}
	};
	
	struct QuestDouble
	{
		unsigned int type{};
		unsigned int amount{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(type), CEREAL_NVP(amount));
		}
	};

	struct ItemRewardStruct
	{

	};

	enum QuestProps {
		// tasks - QuestDouble
		KillAmount = 0,
		GatherAmount = 1,
		// talk for completion - int
		TalkTo = 2,
		// rewards - int
		ExpReward = 3, // int
		MoneyReward = 4, // Wel / Lira - int
		AchievementReward = 5, // int
		EquipReward = 6, // EquipDbStruct
		ItemsReward = 7, 
	};

	struct QuestProperty
	{
		QuestProps key{};
		std::variant<int, std::string, QuestDouble, EquipDbStruct> value;
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(key), CEREAL_NVP(value));
		}
	};

	//using QuestPropertiesMap = std::vector<std::tuple<QuestProps, std::variant<int, std::string, QuestDouble>>>;
	using QuestPropertiesMap = std::vector<QuestProperty>;
	//using QuestPropertiesMapPair = std::pair<QuestProps, std::variant<int, std::string, QuestDouble>>;
	using QuestPropertiesMapPair = QuestProperty;

	struct QuestData
	{
		std::string name;
		std::string description;
		unsigned int req_lvl;
		QuestPropertiesMap quest_properties;

		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(name), CEREAL_NVP(description), CEREAL_NVP(req_lvl), CEREAL_NVP(quest_properties));
		}
	};

}

