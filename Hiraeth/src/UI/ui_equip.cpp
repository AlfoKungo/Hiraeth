#include "ui_equip.h"

namespace hiraeth {
	namespace ui {

		UiEquip::UiEquip(maths::vec2 pos, UiKey control_key, game::CharacterStats *character_stats)
			: UiWindow(maths::Rectangle{ pos, {184, 290} }, control_key),
		m_CharacterStats{character_stats},
		m_ItemsPositions{
			{SRL::EquipItemType::Weapon, {108,129}},
			{SRL::EquipItemType::Hat, {43,228}},
			{SRL::EquipItemType::FaceAcc, {43,195}},
			{SRL::EquipItemType::EyeAcc, {43,162}},
			{SRL::EquipItemType::Top, {43,129}},
			{SRL::EquipItemType::Bottom, {43,96}},
			{SRL::EquipItemType::Cape, {9,129}},
			{SRL::EquipItemType::Glove, {9,97}},
			{SRL::EquipItemType::Shoes, {76,64}},
			{SRL::EquipItemType::Earrings, {108,162}},
			{SRL::EquipItemType::Shoulder, {141,162}},
		}
		{
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(0, 0), graphics::TextureManager::Load("Assets/UI/Equip/Equip.character.backgrnd.png")));
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(6, 5), graphics::TextureManager::Load("Assets/UI/Equip/Equip.character.backgrnd2.png")));
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(10, 32), graphics::TextureManager::Load("Assets/UI/Equip/Equip.character.backgrnd3.png")));

			fillGroup();
		}

		void UiEquip::mouse_moved(float mx, float my, maths::vec2 mousePos)
		{
			//for (auto& [key, item] : m_Equips)
			//	item->setDrawDetails(false);
			auto result_item = std::find_if(std::begin(m_ItemsPositions),
				std::end(m_ItemsPositions), [&](auto const& element)
			{ return maths::Rectangle{ element.second, maths::vec2{32} }.Contains(mousePos); });
			if (result_item != std::end(m_ItemsPositions))
			{
				if (m_Equips.find((*result_item).first) != m_Equips.end())
					m_Equips[(*result_item).first]->setDrawDetails(true);
			}
		}

		void UiEquip::fillGroup()
		{
			m_ForegroundGroup->clear();
			for (auto& [key, equip] : m_Equips)
			{
				m_ForegroundGroup->add_ref(equip);
			}
			//m_ForegroundGroup->add(&m_Equips);
		}

		void UiEquip::StatsUpdated()
		{
			fillGroup();
		}

		void UiEquip::mouse_left_clicked(maths::vec2 mousePos)
		{
		}

	}
}
