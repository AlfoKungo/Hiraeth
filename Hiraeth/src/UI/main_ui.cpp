#include "main_ui.h"
#include <functional>

namespace hiraeth {
	namespace ui {

		MainUi::MainUi()
			: m_CharacterStats(),
			m_StatsStruct(m_CharacterStats.getStatsStruct_()),
			m_Layer(new graphics::Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag")),
		m_GraphicGroup(new graphics::Group(maths::vec2(-555, -450))),
		m_LabelGroup(new graphics::Group(maths::vec2(-555, -450)))
		{
			EventManager *m_EventManager = EventManager::Instance();
			m_EventManager->subscribe(StatsUpdate, this, &MainUi::StatsUpdated);
			fill_stats_group();
			m_Layer.add(new graphics::Label("arial", 11, m_StatsStruct->Job, { -730, -429 }, 0xff70cdd0));
			m_Layer.add(new graphics::Label("arial", 11, m_StatsStruct->Name, { -730, -441 }, 0xffffffff));
			m_Layer.add(m_LabelGroup);
			m_Layer.add(new graphics::Sprite(maths::vec2(-551, -447), graphics::TextureManager::Load("Assets/UI/MainUi/mainBar.gaugeCover.png")));
			m_Layer.add(m_GraphicGroup);
			m_Layer.add(new graphics::Sprite(maths::vec2(-580, -448), graphics::TextureManager::Load("Assets/UI/MainUi/mainBar.gaugeBackgrd.png")));
			m_Layer.add(new graphics::Sprite(maths::vec2(-797, -430), graphics::TextureManager::Load("Assets/UI/MainUi/mainBar.lvCover.png")));
			m_Layer.add(new graphics::Sprite(maths::vec2(-799, -448), graphics::TextureManager::Load("Assets/UI/MainUi/mainBar.lvBacktrnd.png")));
			m_Layer.add(new graphics::Sprite(maths::vec2(-800, -450), graphics::TextureManager::Load("Assets/UI/MainUi/mainBar.backgrnd.png")));
		}

			//void MainUi::StatsUpdatedT(int X)
			//{
			//	m_CharacterStats.getStatsStruct_()->Hp = X;
			//	StatsUpdated();
			//}

		void MainUi::update()
		{
			m_Layer.update();
		}

		void MainUi::draw() const
		{
			m_Layer.render();
		}

		void MainUi::StatsUpdated()
		{
			fill_stats_group();
		}

		void MainUi::fill_stats_group()
		{
			m_GraphicGroup->clear();
			m_LabelGroup->clear();
			float relativeHp = float(m_StatsStruct->Hp) / m_StatsStruct->MaxHp;
			m_Hp = new graphics::Sprite(maths::vec2(3, 19), relativeHp * 139, 12, 0xff0000ff);
			m_GraphicGroup->add(m_Hp);
			m_LabelGroup->add(new graphics::Label("arial", 11, create_stats_string(m_StatsStruct->Hp, m_StatsStruct->MaxHp), { 140, 21 }, 0xffffffff, graphics::Label::Alignment::RIGHT));
			float relativeMp = float(m_StatsStruct->Mp) / m_StatsStruct->MaxMp;
			m_Mp = new graphics::Sprite(maths::vec2(174, 19), relativeMp * 139, 12, 0xfff5b626);
			m_GraphicGroup->add(m_Mp);
			m_LabelGroup->add(new graphics::Label("arial", 11, create_stats_string(m_StatsStruct->Mp, m_StatsStruct->MaxMp), { 311, 21 }, 0xffffffff, graphics::Label::Alignment::RIGHT));
			float relativeExp = float(m_StatsStruct->Exp) / m_StatsStruct->MaxExp;
			m_Exp = new graphics::Sprite(maths::vec2(3, 3), relativeExp * 310, 12, 0xff18dba9);
			m_GraphicGroup->add(m_Exp);
			m_LabelGroup->add(new graphics::Label("arial", 11, create_stats_string(m_StatsStruct->Exp, m_StatsStruct->MaxExp), { 311, 5 }, 0xffffffff, graphics::Label::Alignment::RIGHT));
		}

		std::string MainUi::create_stats_string(unsigned int value, unsigned int maxValue) const
		{
				return "[" + std::to_string(value) + " / " + std::to_string(maxValue) + "]";
		}
	}
}