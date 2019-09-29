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
			unsigned int m_TypeId{};

		protected:
			//graphics::Group m_DetailsBox;
			graphics::Sprite m_DetailsBoxSprite;
			graphics::Label m_DetailsBoxLabelHeader, m_DetailsBoxLabelContent;
			bool m_IsDrawDetails;
		public:
			//Item(maths::vec2 pos, SRL::ItemData item_data, const std::vector<physics::FootHold>& foot_holds);
			ItemHold(unsigned int item_type_id, SRL::BasicItemInfo item_info,
				SRL::TextureData item_texture_data);

			void draw(graphics::Renderer * renderer) const override;
			void update() override;
			//unsigned int getId() const { return m_Id; }
			void setDrawDetails(bool is_draw_details)
			{
				m_IsDrawDetails = is_draw_details;
				if (is_draw_details)
				{
					m_DetailsBoxSprite.setPosition(m_Bounds.position + maths::vec2(30, -75));
					m_DetailsBoxLabelHeader.setPosition(m_Bounds.position + maths::vec2(40, -10));
					m_DetailsBoxLabelContent.setPosition(m_Bounds.position + maths::vec2(40, -30));
				}
			}
			SRL::ItemTab getTabType() const { return m_BasicItemInfo.type; }
			std::string getName() const { return m_BasicItemInfo.item_name; }
			unsigned int getTypeId() { return m_TypeId; }
			//SRL::ItemPropertiesMap * getItemProperties() { return &m_ItemInfo.item_properties; }
		};
	}
}
