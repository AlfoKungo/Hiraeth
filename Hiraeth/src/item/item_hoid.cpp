#include "item_hold.h"

namespace hiraeth {
	namespace item {

		ItemHold::ItemHold(unsigned int item_type_id,
			SRL::BasicItemInfo item_info, SRL::TextureData item_texture_data)
			: Sprite(maths::vec2{0}, graphics::TextureManager::Load(item_info.item_name, item_texture_data)),
			m_State(InAir),
			m_BasicItemInfo(item_info),
			//m_Id(item_id),
			//m_TypeId(item_id),
			m_DetailsBoxSprite(maths::vec2(30, -75), 180, 80, 0x88331a00),
			m_DetailsBoxLabelHeader("arial", 16, m_BasicItemInfo.item_name, { 40, -10 }, 0xffffffff, graphics::Label::Alignment::LEFT),
			m_DetailsBoxLabelContent("arial", 13, m_BasicItemInfo.item_description, { 40, -30 }, 0xffffffff, graphics::Label::Alignment::LEFT),
			m_IsDrawDetails(false)
		{

		}

		void ItemHold::draw(graphics::Renderer * renderer) const
		{
			graphics::Sprite::draw(renderer);
			if (m_IsDrawDetails)
			{
				m_DetailsBoxSprite.draw(renderer);
				m_DetailsBoxLabelHeader.draw(renderer);
				m_DetailsBoxLabelContent.draw(renderer);
			}
		}

	}
}
