#pragma once
#include <math.h>
#include "graphics/sprite.h"
#include "map/maplayer.h"
#include "basic/a_timer.h"
#include "srl/item_data.h"
#include "graphics/label.h"
#include "graphics/layers/group.h"
//#include "item_data_manager.h"

namespace hiraeth {
	namespace item {

		class ItemHold : public graphics::Sprite
		{
		public:
			enum ItemState {
				InAir,
				OnFloor,
				PickedUp,
				InInventory,
			} m_State;
		private:
			//SRL::ItemInfo m_ItemInfo;
			SRL::BasicItemInfo m_BasicItemInfo;
			//SRL::ItemType m_ItemType{SRL::ItemType::FaceAcc};
			//unsigned int m_Id{};
			unsigned int m_ItemId{};

		protected:
			//graphics::Group m_DetailsBox;
			graphics::Sprite m_DetailsBoxSprite;
			graphics::Label m_DetailsBoxLabelHeader, m_DetailsBoxLabelContent;
			bool m_IsDrawDetails{ false };
		public:
			ItemHold(unsigned int item_type_id, SRL::BasicItemInfo item_info,
				SRL::TextureData item_texture_data);

			void draw(graphics::Renderer * renderer) const override;
			void setDrawDetails(bool is_draw_details, maths::vec2 pos)
			{
				m_IsDrawDetails = is_draw_details;
				if (is_draw_details)
				{
					m_DetailsBoxSprite.setPosition(pos + maths::vec2(30, -75));
					m_DetailsBoxLabelHeader.setPosition(pos + maths::vec2(40, -10));
					m_DetailsBoxLabelContent.setPosition(pos + maths::vec2(40, -30));
				}
			}
			void setDrawDetails(bool is_draw_details)
			{
				setDrawDetails(is_draw_details, m_Bounds.position);
				//m_IsDrawDetails = is_draw_details;
				//if (is_draw_details)
				//{
				//	m_DetailsBoxSprite.setPosition(m_Bounds.position + maths::vec2(30, -75));
				//	m_DetailsBoxLabelHeader.setPosition(m_Bounds.position + maths::vec2(40, -10));
				//	m_DetailsBoxLabelContent.setPosition(m_Bounds.position + maths::vec2(40, -30));
				//}
			}
			SRL::ItemTab getTabType() const { return m_BasicItemInfo.type; }
			std::string getName() const { return m_BasicItemInfo.item_name; }
			unsigned int getItemId() const { return m_ItemId; }
			//SRL::ItemPropertiesMap * getItemProperties() { return &m_ItemInfo.item_properties; }
		};
	}
}
