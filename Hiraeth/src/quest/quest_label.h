#pragma once
#include "graphics/label.h"
#include "srl/quest_data.h"

namespace hiraeth {
	namespace quest {
		
		class QuestLabel : public graphics::Label
		{
			SRL::QuestData m_Data;
			unsigned int m_QuestIndex;
			unsigned int m_LocationIndex;
			
			QuestLabel(SRL::QuestData quest_data, unsigned int quest_index) :
			graphics::Label({ "arial", 19, quest_data.name, 
				//maths::vec2{ 14 , 315 - (float(place_index) - 1) * 17}, 
				maths::vec2{ },
				0xff000000, graphics::Label::Alignment::LEFT }),
			m_Data(quest_data),
			m_QuestIndex(quest_index),
			m_LocationIndex(quest_index)
			{
				
			}
		public:
			QuestLabel(unsigned int quest_index)
				: QuestLabel(SRL::deserial<SRL::QuestData>("quest", quest_index), quest_index)
			{}
			unsigned int get_index() const { return m_QuestIndex; }
			unsigned int get_location_index() const { return m_LocationIndex; }
			void relocateByIndex(unsigned int index)
			{
				m_LocationIndex = index;
				setPosition(maths::vec2{ 14, 315 - (float(index) - 1) * 17 });
			}
			SRL::QuestData getQuestData() { return m_Data; }
		};

	}
}
