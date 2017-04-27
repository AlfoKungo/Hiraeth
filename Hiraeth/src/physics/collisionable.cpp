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

        bool Collisionable::check_if_still_on_foothold()
        {
            physics::FootHold foothold = m_MapLayer->m_FootHolds.at(m_Foothold);
            return (m_Box.GetMaximumBound().x > foothold.p1.x && m_Box.GetMinimumBound().x < foothold.p2.x);
        }

        maths::vec2 Collisionable::force_by_vertical_foothold(const maths::vec2& force, int footholdIndex)
        {
            physics::FootHold foothold = m_MapLayer->m_FootHolds.at(footholdIndex);
            float x_force = force.x;
            if (force.x > 0)
                x_force = foothold.p1.x - m_Box.width - m_Box.x;
            else if (force.x < 0)
                x_force = foothold.p1.x - m_Box.x;
            return maths::vec2(x_force, force.y);
        }

        maths::vec2 Collisionable::set_y_by_foothold(const maths::vec2& force)
        {
            physics::FootHold foothold = m_MapLayer->m_FootHolds.at(m_Foothold);
            float d = 0;
            if (foothold.p1.x != foothold.p2.x)
                d = (foothold.p1.x - m_Box.x) / (foothold.p1.x - foothold.p2.x);
            float y_pos = d * foothold.p2.y + (1 - d) * foothold.p1.y - m_Box.y;
            return maths::vec2(force.x, y_pos);
            //return maths::vec2(m_Box.x, y_pos);
        }

        physics::CollisionStruct Collisionable::analyze_collision(maths::Rectangle char_rec, maths::vec2 char_speed)
        {
            std::vector<physics::FootHold> *m_FootHolds = &m_MapLayer->m_FootHolds;
            physics::CollisionStruct collision = { NO_FOOTHOLD, NO_FOOTHOLD };
            float x_force = char_speed.x, y_force = char_speed.y;
            maths::Rectangle next_char_rec = char_rec;
            next_char_rec.position += char_speed;

            for (int i = 0; i < m_FootHolds->size(); i++)
            {
                if (m_FootHolds->at(i).LinesIntersect(char_rec.position, next_char_rec.position))
                    if (y_force < 0 && m_FootHolds->at(i).LinesIntersect(char_rec.position, next_char_rec.position))
                        if (m_FootHolds->at(i).is_solid())
                            //if (m_FootHolds->at(i).Intersects(next_char_rec) && !m_FootHolds->at(i).Intersects(char_rec)
                        {
                            collision.x = i;
                        }
                //if (!m_FootHolds->at(i).Intersects(char_rec) && m_FootHolds->at(i).Intersects(next_char_rec)
                if (m_FootHolds->at(i).Intersects(next_char_rec)
                    && !m_FootHolds->at(i).is_solid())
                {
                     collision.y = i;
                }
            }
            return collision;
        }
    }
}