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

#define PICK_UP_TIME 5.5f
#define EXPIRING_TIME 10.0f
#define EXPIRE_FADE_TIME 0.75f

		class Item : public graphics::Sprite
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
			const maths::Rectangle * m_CharRec;
			maths::vec2 m_Force;
			const std::vector<physics::FootHold>& m_FootHolds;
			float m_OsciliateYPos;
			ATimer m_Timer;
			bool m_IsExpiring = false;
			unsigned int m_Id{};
			unsigned int m_TypeId{};

		protected:
			//graphics::Group m_DetailsBox;
			graphics::Sprite m_DetailsBoxSprite;
			graphics::Label m_DetailsBoxLabelHeader, m_DetailsBoxLabelContent;
			bool m_IsDrawDetails;
		public:
			//Item(maths::vec2 pos, SRL::ItemData item_data, const std::vector<physics::FootHold>& foot_holds);
			Item(maths::vec2 pos, unsigned int item_type_id, SRL::BasicItemInfo item_info, SRL::TextureData item_texture_data, 
				const std::vector<physics::FootHold>& foot_holds, unsigned int item_id);

			void draw(graphics::Renderer * renderer) const override;
			void update() override;
			bool isReachedFloor(maths::vec2 org, maths::vec2 force);
			bool hasExpired();
			void startExpire()
			{
				m_IsExpiring = true;
				m_Timer.reSet(EXPIRE_FADE_TIME);
			}
			bool hasBeenTaken();
			void pickUp(const maths::Rectangle * char_rec);
			bool isAbleToPickUp() const { return m_State == OnFloor && !m_IsExpiring; }
			bool isPickedUp() const { return (m_State == PickedUp) && (m_Timer.timeRemain() > PICK_UP_TIME - 0.1f); }
			unsigned int getId() const { return m_Id; }
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