#include "ui_equip.h"

namespace hiraeth {
	namespace ui {

		UiEquip::UiEquip(maths::vec2 pos, UiKey control_key)
			: UiWindow(maths::Rectangle(pos.x, pos.y, 184, 290), control_key),
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
			//EventManager *m_EventManager = EventManager::Instance();
			//m_EventManager->subscribe(StatsUpdate, this, &UiEquip::StatsUpdated);
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(0, 0), graphics::TextureManager::Load("Assets/UI/Equip/Equip.character.backgrnd.png")));
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(6, 5), graphics::TextureManager::Load("Assets/UI/Equip/Equip.character.backgrnd2.png")));
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(10, 32), graphics::TextureManager::Load("Assets/UI/Equip/Equip.character.backgrnd3.png")));



			fillGroup();
		}

		void UiEquip::fillGroup()
		{
			m_ForegroundGroup->clear();
			m_ForegroundGroup->add(&m_Equips);
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
