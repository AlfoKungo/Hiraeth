#include "collisionable.h"

namespace hiraeth {
    namespace physics {

        Collisionable::Collisionable(maths::Rectangle& box, map::MapLayer* m_MapLayer)
            : m_Foothold(NO_FOOTHOLD), m_MapLayer(m_MapLayer), m_Box(box)
        {
        }

	    Collisionable::Collisionable(maths::Rectangle& empty_box, map::MapLayer* m_MapLayer, 
			unsigned int foothold)
            : m_Foothold(foothold), m_MapLayer(m_MapLayer), m_Box(empty_box)
	    {
			m_Force = set_y_by_foothold({ 0 });
	    }

	    void Collisionable::analyze_collision()
        {
            m_LastFoothold = m_Foothold;
            maths::vec2 FootHold = maths::vec2(NO_FOOTHOLD);
            if (m_Foothold != NO_FOOTHOLD)
            {
                if (m_Force.y > 1)
                    m_Foothold = NO_FOOTHOLD;
                else if (!check_if_still_on_foothold())
                    m_Foothold = NO_FOOTHOLD;
                else
                {
                    m_Force = set_y_by_foothold(m_Force);
                }
            }
            else
            {
                m_Foothold = analyze_collision_x(maths::vec2(0,m_Force.y));
                if (m_Foothold != NO_FOOTHOLD)
                {
                    //set_foothold(FootHold.x);
                    m_Force = set_y_by_foothold(m_Force);
                }
            }
            FootHold.y = analyze_collision_y(m_Force);
            if (FootHold.y != NO_FOOTHOLD)
            {
                m_Force = force_by_vertical_foothold(m_Force, FootHold.y);
            }
            move(m_Force);
            if (m_Foothold != NO_FOOTHOLD)
            {
                m_Force.y = 0;
            }

        }

        bool Collisionable::check_if_still_on_foothold() const
        {
            const physics::FootHold& foothold = m_MapLayer->getFootHolds().at(m_Foothold);
            return (m_Box.GetBottomMiddle().x > foothold.p1.x && m_Box.GetBottomMiddle().x < foothold.p2.x);
        }

        maths::vec2 Collisionable::force_by_vertical_foothold(const maths::vec2& force, int footholdIndex) const
        {
	        const auto foothold = m_MapLayer->getFootHolds().at(footholdIndex);
	        auto x_force = force.x;
            if (force.x > 0)
                x_force = foothold.p1.x - m_Box.width - m_Box.x;
            else if (force.x < 0)
                x_force = foothold.p1.x - m_Box.x;
            return {x_force, force.y};
        }

        maths::vec2 Collisionable::set_y_by_foothold(const maths::vec2& force) const
        {
            physics::FootHold foothold = m_MapLayer->getFootHolds().at(m_Foothold);
            float d = 0;
            if (foothold.p1.x != foothold.p2.x)
                d = (foothold.p1.x - m_Box.x) / (foothold.p1.x - foothold.p2.x);
	        const float y_force = d * foothold.p2.y + (1 - d) * foothold.p1.y - m_Box.y;
            return {force.x, y_force};
        }

        int Collisionable::analyze_collision_x(const maths::vec2& char_speed) const
        {
			if (char_speed.y > 0)
				return NO_FOOTHOLD;

            const std::vector<physics::FootHold>& m_FootHolds = m_MapLayer->getFootHolds();
            const maths::vec2 char_pos = m_Box.GetBottomMiddle();
	        const maths::vec2 next_char_pos = (m_Box + char_speed).GetBottomMiddle();

            for (int index = 0; index < m_FootHolds.size(); ++index)
            {
                if (m_FootHolds.at(index).LinesIntersect(char_pos, next_char_pos))
                        if (m_FootHolds.at(index).is_solid())
                            //if (m_FootHolds->at(index).Intersects(next_char_rec) && !m_FootHolds->at(index).Intersects(char_rec)
                        {
                            return index;
                        }
            }
            return NO_FOOTHOLD;
        }

        int Collisionable::analyze_collision_y(const maths::vec2& char_speed) const
        {
            const std::vector<physics::FootHold>& m_FootHolds = m_MapLayer->getFootHolds();
            maths::Rectangle next_char_rec = m_Box + char_speed;

            for (int index = 0; index < m_FootHolds.size(); ++index)
            {
                if (m_FootHolds.at(index).Intersects(next_char_rec)
                    //&& !m_FootHolds->at(index).Intersects(char_rec)
                    && next_char_rec.y != m_FootHolds.at(index).p2.y 
                    && !m_FootHolds.at(index).is_solid()
                    )
                {
                     return index;
                }
            }
            return NO_FOOTHOLD;
        }
    }
}