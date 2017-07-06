#include "ui_stats.h"

namespace hiraeth {
	namespace ui {

		UiStats::UiStats(maths::vec2 pos, input::Controls control_key,
			game::CharacterStats *character_stats)
			: UiWindow(maths::Rectangle(pos.x, pos.y, 212, 373), control_key),
		m_StatsStruct(character_stats->getStatsStruct_()), m_DetailsStruct(character_stats->getDetailsStruct_())
		{
			//character_stats->registerToStatsUpdate(this);
			EventManager *m_EventManager = EventManager::Instance();
			m_EventManager->subscribe(StatsUpdate, this, &UiStats::StatsUpdated);
			m_BackgroundGroup.add(new graphics::Sprite(0, 0, graphics::TextureManager::Load("Assets/UI/Stats/Stat.main.backgrnd.png")));
			m_BackgroundGroup.add(new graphics::Sprite(6, 5, graphics::TextureManager::Load("Assets/UI/Stats/Stat.main.backgrnd2.png")));
			m_BackgroundGroup.add(new graphics::Sprite(7, 114, graphics::TextureManager::Load("Assets/UI/Stats/Stat.main.backgrnd3.png")));
			fillGroup();
		}

		void UiStats::fillGroup()
		{
			m_ForegroundGroup.clear();
			//m_Group.add(new graphics::Sprite(120, 120, graphics::TextureManager::Load("Stat.main.BtAuto.normal.1.png")));
			m_ForegroundGroup.add(new graphics::SpritedRenderable(maths::vec3(120, 120, 0), 4, 0.15f, true, graphics::TextureManager::Load("Assets/UI/Stats/Stat.main.BtAuto.normal.png"), maths::vec2(0)));
			m_ForegroundGroup.add(new graphics::Label("arial", 13, m_StatsStruct->Luk, 74, 44, 0xff000000));
			m_ForegroundGroup.add(new graphics::Label("arial", 13, m_StatsStruct->Int, 74, 62, 0xff000000));
			m_ForegroundGroup.add(new graphics::Label("arial", 13, m_StatsStruct->Dex, 74, 80, 0xff000000));
			m_ForegroundGroup.add(new graphics::Label("arial", 13, m_StatsStruct->Str, 74, 98, 0xff000000));
			m_ForegroundGroup.add(new graphics::Label("arial", 13, m_StatsStruct->AbilityPoints, 78, 124, 0xff000000));
			m_ForegroundGroup.add(new graphics::Label("arial", 13, m_StatsStruct->Fame, 74, 167, 0xff000000));
			m_ForegroundGroup.add(new graphics::Label("arial", 13, m_StatsStruct->Mp, 74, 185, 0xff000000));
			m_ForegroundGroup.add(new graphics::Label("arial", 13, m_StatsStruct->Hp, 74, 203, 0xff000000));
			m_ForegroundGroup.add(new graphics::Label("arial", 13, m_StatsStruct->Guild, 74, 221, 0xff000000));
			m_ForegroundGroup.add(new graphics::Label("arial", 13, m_StatsStruct->HonorExp, 74, 239, 0xff000000));
			m_ForegroundGroup.add(new graphics::Label("arial", 13, m_StatsStruct->HonorLvl, 74, 257, 0xff000000));
			m_ForegroundGroup.add(new graphics::Label("arial", 13, m_StatsStruct->Exp, 74, 275, 0xff000000));
			m_ForegroundGroup.add(new graphics::Label("arial", 13, m_StatsStruct->Level, 74, 293, 0xff000000));
			m_ForegroundGroup.add(new graphics::Label("arial", 13, m_StatsStruct->Job, 74, 311, 0xff000000));
			m_ForegroundGroup.add(new graphics::Label("arial", 13, m_StatsStruct->Name, 74, 329, 0xff000000));
		}

		void UiStats::StatsUpdated()
		{
				static_cast<graphics::Label*>(m_ForegroundGroup.m_Renderables.at(11))->setText(m_StatsStruct->Hp);
		}

	}
}
