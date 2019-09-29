#include "item.h"

namespace hiraeth {
	namespace item {

		Item::Item(maths::vec2 pos, unsigned int item_type_id, SRL::BasicItemInfo item_info, SRL::TextureData item_texture_data, 
			const std::vector<physics::FootHold>& foot_holds, unsigned int item_id)
			: Sprite(pos, graphics::TextureManager::Load(item_info.item_name, item_texture_data)),
			m_State(InAir),
			m_BasicItemInfo(item_info),
			m_Force(0, 7),
			m_FootHolds(foot_holds),
			m_Id(item_id),
			m_TypeId(item_id),
			m_DetailsBoxSprite(maths::vec2(30, -75), 180, 80, 0x88331a00),
			m_DetailsBoxLabelHeader("arial", 16, m_BasicItemInfo.item_name, { 40, -10 }, 0xffffffff, graphics::Label::Alignment::LEFT),
			m_DetailsBoxLabelContent("arial", 13, m_BasicItemInfo.item_description, { 40, -30 }, 0xffffffff, graphics::Label::Alignment::LEFT),
			m_IsDrawDetails(false)
		{

		}

		void Item::draw(graphics::Renderer * renderer) const
		{
			graphics::Sprite::draw(renderer);
			if (m_IsDrawDetails)
			{
				m_DetailsBoxSprite.draw(renderer);
				m_DetailsBoxLabelHeader.draw(renderer);
				m_DetailsBoxLabelContent.draw(renderer);
			}
		}

		void Item::update()
		{
			switch (m_State)
			{
			case InAir:
				m_Force.y -= 0.6f;
				m_Bounds.position += m_Force;
				if (isReachedFloor(maths::vec2(0, 10), m_Force))
				{
					m_State = OnFloor;
					m_Timer.reSet(EXPIRING_TIME);
				}
				break;
			case OnFloor:
				m_Bounds.position.y = m_OsciliateYPos + sin(m_Timer.timeRemain() * 3.5f) * 3;
				break;
			case InInventory:
				break;
			case PickedUp:
				float time_remains= m_Timer.timeRemain();
				if (time_remains < 0.0f)
					time_remains = 0.0f;
				maths::vec2 char_pos = m_CharRec->GetBottomMiddle() + maths::vec2(0,15);
				float relativeTimePassed = pow(1.0f - (time_remains / PICK_UP_TIME), 1.0f);
				m_Color = 0x00ffffff | (uint32_t(256 * (1.0f - relativeTimePassed * 10)) << 24);

				m_Force = ((relativeTimePassed * (char_pos - m_Bounds.GetMiddle())) +
					(1.0f - relativeTimePassed) * m_Force) *
					(relativeTimePassed + 2.25f) / 3.25f;
				m_Bounds.position.y += m_Force.y;
				float m_Forcex = (char_pos.x - m_Bounds.GetMiddle().x) * (relativeTimePassed / 0.2f);
				m_Bounds.position.x += m_Forcex;
				break;
			}
			if (m_IsExpiring)
			{
				m_Color = 0x00ffffff | (uint32_t(256 * (m_Timer.timeRemain() / EXPIRE_FADE_TIME)) << 24);
			}
			Sprite::update();
		}

		bool Item::isReachedFloor(maths::vec2 org, maths::vec2 force)
		{
			if (force.y > 0)
				return false;

			const maths::vec2 current_pos = (m_Bounds + org).GetBottomMiddle();
			const maths::vec2 next_pos = (m_Bounds + org + force).GetBottomMiddle();

			for (int i = 0; i < m_FootHolds.size(); i++)
			{
				if (m_FootHolds.at(i).LinesIntersect(current_pos, next_pos))
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
			//if ((m_State == OnFloor) && m_Timer.timeRemain() < EXPIRE_FADE_TIME)
			//	m_IsExpiring = true;
			return ((m_State == OnFloor) && m_Timer.hasExpired());
		}

		bool Item::hasBeenTaken()
		{
			if ((m_State == PickedUp)) {
				if ((m_Force.y < 0 || m_Force.y < 0.1)) {
					if ((abs((m_CharRec->y + 15) - m_Bounds.y) < 3.0f))
					{
						m_Color = 0xffffffff;
						m_State = InInventory;
						m_IsExpiring = false;
						return true;
					}
				}
			}
			return false;
		}

		void Item::pickUp(const maths::Rectangle * char_rec)
		{
			m_CharRec = char_rec;
			m_State = PickedUp;
			m_Force = maths::vec2(0, 17);
			//m_Force = maths::vec2(0, 6.0f);
			m_Timer.reSet(PICK_UP_TIME);
		}
	}
}