#pragma once

namespace SRL {

	enum QuestTab {
		Available = 0,
		InProgress = 1,
		Completed = 2,
		Recommended = 3,
	};

	//enum QuestProps {
	//	// tasks
	//	KillAmount = 0,
	//	GatherAmount = 1,
	//	// talk for completion
	//	TalkTo = 2,
	//	// rewards
	//	ExpReward = 3,
	//	MoneyReward = 4, // Wel / Lira
	//	AchievementReward = 5,
	//};

	//using QuestPropertiesMap = std::map<QuestProps, std::variant<int, std::string>>;
	//using QuestPropertiesMapPair = std::pair<QuestProps, std::variant<int, std::string>>;

}