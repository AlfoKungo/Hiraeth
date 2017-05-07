#include "collisionable.h"

namespace hiraeth {
    namespace physics {

        Collisionable::Collisionable(maths::Rectangle& box, map::MapLayer* m_MapLayer)
            : m_MapLayer(m_MapLayer), m_Box(box), m_Foothold(NO_FOOTHOLD)
        {
        }

        void Collisionable::set_foothold(int foothold_index)
        {
            m_Foothold = foothold_index;
        }

        bool Collisionable::check_if_still_on_foothold() const
        {
            physics::FootHold foothold = m_MapLayer->m_FootHolds.at(m_Foothold);
            return (m_Box.GetBottomMiddle().x > foothold.p1.x && m_Box.GetBottomMiddle().x < foothold.p2.x);
        }

        maths::vec2 Collisionable::force_by_vertical_foothold(const maths::vec2& force, int footholdIndex) const
       {
            physics::FootHold foothold = m_MapLayer->m_FootHolds.at(footholdIndex);
            float x_force = force.x;
            if (force.x > 0)
                x_force = foothold.p1.x - m_Box.width - m_Box.x;
            else if (force.x < 0)
                x_force = foothold.p1.x - m_Box.x;
            return maths::vec2(x_force, force.y);
        }

        maths::vec2 Collisionable::set_y_by_foothold(const maths::vec2& force) const
        {
            physics::FootHold foothold = m_MapLayer->m_FootHolds.at(m_Foothold);
            float d = 0;
            if (foothold.p1.x != foothold.p2.x)
                d = (foothold.p1.x - m_Box.x) / (foothold.p1.x - foothold.p2.x);
            float y_force = d * foothold.p2.y + (1 - d) * foothold.p1.y - m_Box.y;
            return maths::vec2(force.x, y_force);
            //return maths::vec2(m_Box.x, y_pos);
        }

        int Collisionable::analyzeCollisionX(const maths::Rectangle& char_rec, const maths::vec2& char_speed) const
        {
            std::vector<physics::FootHold> *m_FootHolds = &m_MapLayer->m_FootHolds;
            maths::vec2 char_pos = char_rec.GetBottomMiddle();
            maths::vec2 next_char_pos = (char_rec + char_speed).GetBottomMiddle();

            for (int i = 0; i < m_FootHolds->size(); i++)
            {
                if (m_FootHolds->at(i).LinesIntersect(char_pos, next_char_pos))
                    if (char_speed.y < 0)
                        if (m_FootHolds->at(i).is_solid())
                            //if (m_FootHolds->at(i).Intersects(next_char_rec) && !m_FootHolds->at(i).Intersects(char_rec)
                        {
                            return i;
                        }
            }
            return -1;
        }

        int Collisionable::analyzeCollisionY(const maths::Rectangle& char_rec, const maths::vec2& char_speed) const
        {
            std::vector<physics::FootHold> *m_FootHolds = &m_MapLayer->m_FootHolds;
            maths::Rectangle next_char_rec = char_rec;
            next_char_rec.position += char_speed;

            for (int i = 0; i < m_FootHolds->size(); i++)
            {
                if (m_FootHolds->at(i).Intersects(next_char_rec)
                    //&& !m_FootHolds->at(i).Intersects(char_rec)
                    && next_char_rec.y != m_FootHolds->at(i).p2.y 
                    && !m_FootHolds->at(i).is_solid()
                    )
                {
                     return i;
                }
            }
            return -1;
        }
    }
}