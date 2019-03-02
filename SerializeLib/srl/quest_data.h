#pragma once
#include "cereal/types/variant.hpp"
#include "cereal/types/map.hpp"
#include "cereal/cereal.hpp"

namespace SRL {

	enum QuestTab {
		Available = 0,
		InProgress = 1,
		Completed = 2,
		Recommended = 3,
	};

	struct QuestDouble
	{
		unsigned int type{};
		unsigned int amount{};
		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(type), CEREAL_NVP(amount));
		}
	};

	enum QuestProps {
		// tasks - QuestDouble
		KillAmount = 0,
		GatherAmount = 1,
		// talk for completion - int
		TalkTo = 2,
		// rewards - int
		ExpReward = 3,
		MoneyReward = 4, // Wel / Lira
		AchievementReward = 5,
	};

	using QuestPropertiesMap = std::map<QuestProps, std::variant<int, std::string, QuestDouble>>;
	using QuestPropertiesMapPair = std::pair<QuestProps, std::variant<int, std::string, QuestDouble>>;

	struct QuestData
	{
		QuestPropertiesMap quest_properties;

		template<class A> void serialize(A& ar) {
			ar(CEREAL_NVP(quest_properties));
		}
	};

}

