#pragma once
#include <math.h>
#include "graphics/sprite.h"
#include "map/maplayer.h"
#include "basic/a_timer.h"
#include "item_data_manager.h"

namespace hiraeth {
	namespace item {

#define PICK_UP_TIME 1.5f
		class Item : public graphics::Sprite
		{
			enum ItemState {
				InAir,
				OnFloor,
				PickedUp,
				InInventory,
			} m_State;
			const maths::Rectangle * m_CharRec;
			maths::vec2 m_Force;
			const std::vector<physics::FootHold>& m_FootHolds;
			float m_OsciliateYPos;
			ATimer m_Timer;
		public:
			Item(maths::vec2 pos, graphics::Texture* texture, const std::vector<physics::FootHold>& foot_holds)
				: Sprite(pos.x, pos.y, texture), m_Force(0, 7),
				m_FootHolds(foot_holds),
				m_State(InAir)
			{}
			void setPosition(maths::vec2 newPos)
			{
				m_Bounds.position = newPos;
			}
			void update() override;
			bool checkIfOnFloor(maths::vec2 org, maths::vec2 force);
			bool hasExpired();
			bool hasBeenTaken();
			void pickUp(const maths::Rectangle * char_rec);
			bool isAbleToPickUp() { return m_State == OnFloor; }
		};
	}
}