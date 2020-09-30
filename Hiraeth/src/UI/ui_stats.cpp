#include "hrth_pch.h"
#include "ui_stats.h"

namespace hiraeth {
	namespace ui {

		UiStats::UiStats(maths::vec2 pos, UiKey control_key,
			game::CharacterStats *character_stats)
			: UiWindow(maths::Rectangle(pos.x, pos.y, 212, 373), control_key),
			m_StatsStruct(character_stats->getStatsStruct_()), m_DetailsStruct(character_stats->getDetailsStruct_()),
		m_DetailsGroup{new graphics::Group{}}  
		{
			EventManager *m_EventManager = EventManager::Instance();
			m_EventManager->subscribe(StatsUpdate, this, &UiStats::StatsUpdated);
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(0, 0), graphics::TextureManager::Load("Assets/UI/Stats/Stats.main.backgrnd.png")));
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(6, 5), graphics::TextureManager::Load("Assets/UI/Stats/Stats.main.backgrnd2.png")));
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(7, 114), graphics::TextureManager::Load("Assets/UI/Stats/Stats.main.backgrnd3.png")));
			m_DetailsGroup->add(new graphics::Sprite(maths::vec2(212, 0), graphics::TextureManager::Load("Assets/UI/Stats/Stats.detail.backgrnd.png")));
			m_DetailsGroup->add(new graphics::Sprite(maths::vec2(218, 5), graphics::TextureManager::Load("Assets/UI/Stats/Stats.detail.backgrnd2.png")));
			m_DetailsGroup->add(new graphics::Sprite(maths::vec2(223, 35), graphics::TextureManager::Load("Assets/UI/Stats/Stats.detail.backgrnd3.png")));
			m_BackgroundGroup->add(m_DetailsGroup);
			m_DetailsGroup->is_to_draw = false;
			

			add_button(new UiButtonFlashing(maths::vec2(120, 120), "Stats", "BtAuto", std::bind(&UiStats::auto_assign_clicked, this)));
			add_button(new UiButtonSideMenu(maths::vec2(130, 12), "Stats", "BtDetail", std::bind(&UiStats::details_clicked, this)));
			fillGroup();
		}

		void UiStats::fillGroup()
		{
			m_ForegroundGroup->clear();
			m_ForegroundGroup->add(new graphics::Label("arial", 13, m_StatsStruct->Wit, { 74, 44 }, 0xff000000));
			m_ForegroundGroup->add(new graphics::Label("arial", 13, m_StatsStruct->Int, { 74, 62 }, 0xff000000));
			m_ForegroundGroup->add(new graphics::Label("arial", 13, m_StatsStruct->Dex, { 74, 80 }, 0xff000000));
			m_ForegroundGroup->add(new graphics::Label("arial", 13, m_StatsStruct->Str, { 74, 98 }, 0xff000000));
			m_ForegroundGroup->add(new graphics::Label("arial", 13, m_StatsStruct->AbilityPoints, { 78, 124 }, 0xff000000));
			m_ForegroundGroup->add(new graphics::Label("arial", 13, m_StatsStruct->Fame, { 74, 167 }, 0xff000000));
			m_ForegroundGroup->add(new graphics::Label("arial", 13, m_StatsStruct->Mp, { 74, 185 }, 0xff000000));
			m_ForegroundGroup->add(new graphics::Label("arial", 13, m_StatsStruct->Hp, { 74, 203 }, 0xff000000));
			m_ForegroundGroup->add(new graphics::Label("arial", 13, m_StatsStruct->Guild, { 74, 221 }, 0xff000000));
			m_ForegroundGroup->add(new graphics::Label("arial", 13, m_StatsStruct->HonorExp, { 74, 239 }, 0xff000000));
			m_ForegroundGroup->add(new graphics::Label("arial", 13, m_StatsStruct->HonorLvl, { 74, 257 }, 0xff000000));
			m_ForegroundGroup->add(new graphics::Label("arial", 13, m_StatsStruct->Exp, { 74, 275 }, 0xff000000));
			m_ForegroundGroup->add(new graphics::Label("arial", 13, m_StatsStruct->Level, { 74, 293 }, 0xff000000));
			m_ForegroundGroup->add(new graphics::Label("arial", 13, m_StatsStruct->Job, { 74, 311 }, 0xff000000));
			m_ForegroundGroup->add(new graphics::Label("arial", 13, m_StatsStruct->Name, { 74, 329 }, 0xff000000));

			m_DetailsGroup->clear();
			m_DetailsGroup->add(new graphics::Sprite(maths::vec2(212, 0), graphics::TextureManager::Load("Assets/UI/Stats/Stats.detail.backgrnd.png")));
			m_DetailsGroup->add(new graphics::Sprite(maths::vec2(218, 5), graphics::TextureManager::Load("Assets/UI/Stats/Stats.detail.backgrnd2.png")));
			m_DetailsGroup->add(new graphics::Sprite(maths::vec2(223, 35), graphics::TextureManager::Load("Assets/UI/Stats/Stats.detail.backgrnd3.png")));
			m_DetailsGroup->add(new graphics::Label("arial", 13, m_DetailsStruct->Attack, { 284, 358 }, 0xff000000));
			m_DetailsGroup->add(new graphics::Label("arial", 13, m_DetailsStruct->CritRate, { 284, 340 }, 0xff000000));
			m_DetailsGroup->add(new graphics::Label("arial", 13, m_DetailsStruct->WeaponDef, { 284, 322 }, 0xff000000));
			m_DetailsGroup->add(new graphics::Label("arial", 13, m_DetailsStruct->MagicDef, { 284, 304 }, 0xff000000));
			m_DetailsGroup->add(new graphics::Label("arial", 13, m_DetailsStruct->WeaponAcc, { 284, 286 }, 0xff000000));
			m_DetailsGroup->add(new graphics::Label("arial", 13, m_DetailsStruct->MagicAcc, { 284, 268 }, 0xff000000));
			m_DetailsGroup->add(new graphics::Label("arial", 13, m_DetailsStruct->WeaponAvd, { 284, 250 }, 0xff000000));
			m_DetailsGroup->add(new graphics::Label("arial", 13, m_DetailsStruct->MagicAvd, { 284, 232 }, 0xff000000));
			m_DetailsGroup->add(new graphics::Label("arial", 13, m_DetailsStruct->Speed, { 284, 214 }, 0xff000000));
			m_DetailsGroup->add(new graphics::Label("arial", 13, m_DetailsStruct->Jump, { 284, 196 }, 0xff000000));
		}

		void UiStats::StatsUpdated()
		{
			fillGroup();
		}

		void UiStats::mouse_left_clicked(maths::vec2 mousePos)
		{
		}

		void UiStats::auto_assign_clicked()
		{
			m_StatsStruct->Dex += m_StatsStruct->AbilityPoints;
			m_StatsStruct->AbilityPoints = 0;
			StatsUpdated();
			std::cout << "Clicked on Auto Assign Button"<< std::endl;
		}

		void UiStats::details_clicked()
		{
			//m_DrawDetails = !m_DrawDetails;
			m_DetailsGroup->is_to_draw = !m_DetailsGroup->is_to_draw;
			std::cout << "Clicked on Details Button" << std::endl;
		}
	}
}
