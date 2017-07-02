#pragma once
#include "graphics/sprite.h"
#include "map/maplayer.h"
#include "basic/a_timer.h"
#include <math.h>

namespace hiraeth {
	namespace item {

		class Item : public graphics::Sprite
		{
			maths::vec2 m_Force;
			physics::FootHoldIndex m_FootHold = NO_FOOTHOLD;
			const std::vector<physics::FootHold>& m_FootHolds;
			float m_OsciliateYPos;
			ATimer timer;
		public:
			Item(maths::vec2 pos, graphics::Texture* texture, const std::vector<physics::FootHold>& foot_holds)
				: Sprite(pos.x, pos.y, texture), m_Force(0, 7),
				m_FootHolds(foot_holds)
			{}
			void update() override
			{
				if (NO_FOOTHOLD == m_FootHold)
				{
					m_Force.y -= 0.6f;
					m_Bounds.position += m_Force;
					m_FootHold = checkIfOnFloor(maths::vec2(0, 10), m_Force);
					if (NO_FOOTHOLD == m_FootHold)
						timer.reSet(50.0f);
				}
				else
				{
					m_Bounds.position.y = m_OsciliateYPos + sin(timer.timeRemain() * 3.5f) * 3;
				}
				Sprite::update();
			}
			int checkIfOnFloor(maths::vec2 org, maths::vec2 force)
			{
				const maths::vec2 current_pos = (m_Bounds + org).GetBottomMiddle();
				const maths::vec2 next_pos = (m_Bounds + org + force).GetBottomMiddle();

				for (int i = 0; i < m_FootHolds.size(); i++)
				{
					if (m_FootHolds.at(i).LinesIntersect(current_pos, next_pos))
						if (force.y < 0)
							if (m_FootHolds.at(i).is_solid())
							{
								const physics::FootHold& foothold = m_FootHolds.at(i);
								float d = (foothold.p1.x - current_pos.x) / (foothold.p1.x - foothold.p2.x);
								m_OsciliateYPos = d * foothold.p2.y + (1 - d) * foothold.p1.y;
								return i;
							}
				}
				return -1;
			}
		};
	}
}