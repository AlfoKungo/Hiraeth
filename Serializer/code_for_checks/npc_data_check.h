#pragma once
#include <cereal/archives/json.hpp>
#include <string>
#include <fstream>
#include "srl/npc_data.h"

namespace SRL
{
	namespace Checks
	{
		static void create_npc_data()
		{
			const std::string SRL_TYPE{ "npc" };
			{
				unsigned int quest_id = 0;
				SRL::NpcInfo info{
					"Chief Guatamala",
					0,
					-200.0,
					{"Hey, care for some CDs nigga?", "May I interest you in\nsome Kanye?"},
				{
						{0,
						DialogNode{{{"They tryna find TuPaC",{{DialogButton::Next}}},
							{"Don't let them find tupac",{{DialogButton::Next}}},
							{"La da da da",{{DialogButton::Next}}}}, {}}},
						{1,
						DialogNode{{{"I have a small request",{{DialogButton::Next}}},
							{"I need you to kill me some monsters",{{DialogButton::Next}}},
							{"1 slime, 1 shroom, easy isn't it?",{{DialogButton::Cancel},{DialogButton::Accept, DAAddQuest{quest_id}}}}}, {DR_Lvl{5}}}},
					{2,
						DialogNode{{{"I'm waiting for it you bitch ass nigga",{{DialogButton::Next}}}}, {DR_ActiveQuest{quest_id}}}},
					{3,
						DialogNode{{{"Aight you cool\n there's you money",{{DialogButton::ReceiveReward, {DAReceiveReward{quest_id}}}}}},{DR_QuestFulfilled{0}}}},
					{4,
						DialogNode{{{"Get outa my sight you fucking nigger",{{DialogButton::Ok}}}},{DR_DontCont{}} }
					}
				},
					//NpcQuestDataStruct{0, 10, 0}
					NpcUsageDialog{}
				};
				std::ofstream data_file("data/" + SRL_TYPE + "/0/data.json", std::ios::out);
				cereal::JSONOutputArchive arout(data_file);
				arout(CEREAL_NVP(info));
			}

			{
				//using enum DialogButton;
				SRL::NpcInfo info{
					"Ruby PQ",
					0,
					-100.0,
					{"I got an interesting offer for you", "This mission is not an easy one"},
				{
					{0,
					{{{"They tryna find TuPaC",{{DialogButton::Next}}},
						{"Don't let them find tupac",{{DialogButton::Next}}},
						//{"La da da da",{2,1}}}}
						{"La da da da",{{DialogButton::Cancel},{DialogButton::Accept}}}}}},
				{1,
					//{{{"I'm waiting for it you bitch ass nigga",{{DialogButton::Next}}}}, {DR_Pq{0,0,0,0}}}},
					{{{"I'm waiting for it you bitch ass nigga",{{DialogButton::Next}}}}, {}}},
				{2,
					{{{"Aight you cool\nthere's you money",{{DialogButton::ReceiveReward}}}}}},
				{3,
					{{{"Aight you cool\nthere's you money",{{DialogButton::Next}}}}}}
				},
					NpcUsagePq{2,2, 10,2 }
				};
				std::ofstream data_file("data/" + SRL_TYPE + "/1/data.json", std::ios::out);
				cereal::JSONOutputArchive arout(data_file);
				arout(CEREAL_NVP(info));
			}
			{
				SRL::NpcInfo info{
					"Lavie",
					0,
					0.0,
					{"It's sunny today isn't it", "Summer madness"},
				{
				//	{0,
				//	{{{"They tryna find TuPaC",{{DialogButton::Next}}},
				//		{"Don't let them find tupac",{{DialogButton::Next}}},
				//		//{"La da da da",{2,1}}}}
				//		{"La da da da",{{DialogButton::Cancel},{DialogButton::Accept}}}}}},
				//{1,
				//	{{{"I'm waiting for it you bitch ass nigga",{{DialogButton::Next}}}}}},
				//{2,
				//	{{{"Aight you cool\n there's you money",{{DialogButton::ReceiveReward}}}}}},
				//{3,
				//	{{{"Aight you cool\n there's you money",{{DialogButton::Next}}}}}}
				},
					NpcUsageMerchant{{{0,0, 100}, {0,1, 250}, {1,1, 150}, {1,2, 400}}}
				};
				std::ofstream data_file("data/" + SRL_TYPE + "/2/data.json", std::ios::out);
				cereal::JSONOutputArchive arout(data_file);
				arout(CEREAL_NVP(info));
			}
			{
				SRL::NpcInfo info{
					"Magic Luka",
					0,
					100.0,
					{"Too easy isn't it?", "Oh yeah"},
				{
					{0,
					{{{"They tryna find TuPaC",{{DialogButton::Next}}},
						{"Don't let them find tupac",{{DialogButton::Next}}},
						//{"La da da da",{2,1}}}}
						{"La da da da",{{DialogButton::Cancel},{DialogButton::Accept}}}}}},
				{1,
					{{{"I'm waiting for it you bitch ass nigga",{{DialogButton::Next}}}}}},
				{2,
					{{{"Aight you cool\nthere's you money",{
						{DialogButton::ReceiveReward}}}}}},
				{3,
					{{{"Aight you cool\nthere's you money",{{DialogButton::Next}}}}}}
				},
					//NpcJobAdvancementDataStruct{0, 2}
					NpcUsageDialog{}
				};
				std::ofstream data_file("data/" + SRL_TYPE + "/3/data.json", std::ios::out);
				cereal::JSONOutputArchive arout(data_file);
				arout(CEREAL_NVP(info));
			}
			{
				SRL::NpcInfo info{
					"Taxi Driver",
					0,
					200.0,
					{"Can take you anywhere", "just say it"},
				{
					{0,
					{{{"Got Anywhere you wanna go?",{{DialogButton::Next}}},
					//	{"Choose a destination\nhelenia for 50 bucks\ntamopolis for 55 bucks",
					//{{DialogButton::Cancel, DAGoToDialog{1}}, {DialogButton::Accept, DAGoToDialog{2}}}}}}},
					//	{"Choose a destination",{{DialogButton::Cancel, DAGoToDialog{3}}},
					DialogStruct{{"I got the cheapestest prices"}, {{DialogButton::Cancel, DAGoToDialog{3}}},
					{{"Helenia for 50 bucks", DAGoToDialog{1}}, {"Tamopolis for 55 bucks", DAGoToDialog{2}}}}}}},
					//{{"helenia for 50 bucks", DAGoToDialog{1}}, {"tamopolis for 55 bucks", DAGoToDialog{2}}}},
					//{"La da da da",{2,1}}}}
					//{"La da da da",{{DialogButton::Cancel},{DialogButton::Accept}}}}}},
			{1,
				{{{"send my regards to Gil, aight?",{{DialogButton::Ok, DAChangeMap{1}}}}},{DR_DontCont{}}}},
			{2,
				{{{"Tell Malcolm I'll come over next time",{{DialogButton::Ok, DAChangeMap{2}}}}}}},
			{3,
				{{{"See ya around than,\nhave fun",{{DialogButton::Ok}}}}, {}}}
			},
				NpcUsageDialog{}
				};
				std::ofstream data_file("data/" + SRL_TYPE + "/4/data.json", std::ios::out);
				cereal::JSONOutputArchive arout(data_file);
				arout(CEREAL_NVP(info));
			}
		}
	}
}
