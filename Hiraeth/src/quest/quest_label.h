#pragma once
#include "graphics/label.h"
#include "srl/quest_data.h"

namespace hiraeth {
	namespace quest {
		
		class QuestLabel : public graphics::Label
		{
			SRL::QuestData m_Data;
			unsigned int m_QuestIndex;
			
			QuestLabel(SRL::QuestData quest_data, unsigned int quest_index, unsigned int place_index) :
			graphics::Label({ "arial", 19, quest_data.name, 
				maths::vec2{ 14 , 315 - (float(place_index) - 1) * 17}, 
				0xff000000, graphics::Label::Alignment::LEFT }),
			m_Data(quest_data),
			m_QuestIndex(quest_index)
			{
				
			}
		public:
			QuestLabel(unsigned int quest_index, unsigned int place_index)
				: QuestLabel(SRL::deserial<SRL::QuestData>("quest", quest_index), quest_index, place_index)
			{}
			unsigned int get_index() const { return m_QuestIndex; }
			void relocateByIndex(unsigned int index)
			{
				setPosition(maths::vec2{ 14, 315 - (float(index) - 1) * 17 });
			}
			SRL::QuestData getQuestData() { return m_Data; }
		};

	}
}
