#pragma once
#include <math.h>
#include "graphics/sprite.h"
#include "map/maplayer.h"
#include "basic/a_timer.h"
#include "srl/item_data.h"
//#include "item_data_manager.h"

namespace hiraeth {
	namespace item {

#define PICK_UP_TIME 5.5f
#define EXPIRING_TIME 10.0f
#define EXPIRE_FADE_TIME 1.5f

		class Item : public graphics::Sprite
		{
			enum ItemState {
				InAir,
				OnFloor,
				PickedUp,
				InInventory,
			} m_State;

			SRL::ItemInfo m_ItemInfo;
			const maths::Rectangle * m_CharRec;
			maths::vec2 m_Force;
			const std::vector<physics::FootHold>& m_FootHolds;
			float m_OsciliateYPos;
			ATimer m_Timer;
			bool m_IsExpiring = false;
		public:
			Item(maths::vec2 pos, SRL::ItemData item_data, const std::vector<physics::FootHold>& foot_holds);

			void setPosition(maths::vec2 newPos)
			{
				m_Bounds.position = newPos;
			}
			void update() override;
			bool isReachedFloor(maths::vec2 org, maths::vec2 force);
			bool hasExpired();
			bool hasBeenTaken();
			void pickUp(const maths::Rectangle * char_rec);
			bool isAbleToPickUp() { return m_State == OnFloor; }
			bool isPickedUp() { return (m_State == PickedUp) && (m_Timer.timeRemain() > PICK_UP_TIME - 0.1f); }
			SRL::Tab getTabType() const { return m_ItemInfo.type; }
		};
	}
}