#include "item.h"

namespace hiraeth {
	namespace item {

		void Item::update()
		{
			switch (m_State)
			{
			case InAir:
				m_Force.y -= 0.6f;
				m_Bounds.position += m_Force;
				if (checkIfOnFloor(maths::vec2(0, 10), m_Force))
				{
					m_State = OnFloor;
					m_Timer.reSet(50.0f);
				}
				break;
			case OnFloor:
				m_Bounds.position.y = m_OsciliateYPos + sin(m_Timer.timeRemain() * 3.5f) * 3;
				break;
			case PickedUp:
				//m_Force *= 0.8f;
				float time_remains= m_Timer.timeRemain();
				if (time_remains < 0.0f)
					time_remains = 0.0f;

				maths::vec2 char_pos = m_CharRec->GetMiddle();
				float relativeTimePassed = pow(1.0f - (time_remains / PICK_UP_TIME), 0.4f);
				m_Force = (((relativeTimePassed * ((char_pos)-m_Bounds.GetMiddle()))) +
					((1.0f - relativeTimePassed) * m_Force))
					*
					(relativeTimePassed + 0.25f) / 1.25f;
				m_Bounds.position += m_Force;
				break;
			}
			Sprite::update();
		}

		bool Item::checkIfOnFloor(maths::vec2 org, maths::vec2 force)
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
							return true;
						}
			}
			return false;
		}

		bool Item::hasExpired()
		{
			return ((m_State == OnFloor) && m_Timer.isExpired());
		}

		bool Item::hasBeenTaken()
		{
			if ((m_State == PickedUp) && m_CharRec->GetMiddle().Distance(m_Bounds.GetMiddle()) < 4.5f)
			{
				m_State = InInventory;
				return true;
			}
			return false;
		}

		void Item::pickUp(const maths::Rectangle * char_rec)
		{
			m_CharRec = char_rec;
			m_State = PickedUp;
			m_Force = maths::vec2(0, 66);
			m_Timer.reSet(PICK_UP_TIME);
		}
	}
}