#include "collisionable.h"

namespace hiraeth {
    namespace physics {

		Collisionable::Collisionable(maths::Rectangle& box, map::MapLayer* map_layer)
            //: m_Foothold(NO_FOOTHOLD), m_MapLayer(m_MapLayer), m_Box(box)
            : Collisionable(box, map_layer, NO_FOOTHOLD)
        {
        }

		Collisionable::Collisionable(maths::Rectangle& empty_box, map::MapLayer* map_layer,
			int foothold)
			: m_Foothold(foothold), m_MapLayer(map_layer), m_Box(empty_box)
		{
			if (m_Foothold != NO_FOOTHOLD)
				m_Force = set_y_by_foothold({ 0 });
		}

		void Collisionable::analyze_collision()
		{
			m_LastFoothold = m_Foothold;
			if (m_Foothold != NO_FOOTHOLD)
			{
				if (m_Force.y > 1)
					m_Foothold = NO_FOOTHOLD;
				else
				{
					if (!check_if_still_on_foothold())
					{
						m_Force = set_y_by_foothold(m_Force);
						m_Foothold = NO_FOOTHOLD;
						m_Foothold = find_next_foothold_x(m_Force);
						//if (m_Foothold == NO_FOOTHOLD)
						//{
 							//std::cout << "deleted foothold and now is" << m_Foothold << std::endl;
							//m_Foothold = m_LastFoothold;
							//m_Force = set_y_by_foothold(m_Force);
							//m_Foothold = find_next_foothold_x(m_Force);
						//}
					}
					//else
					if (m_Foothold != NO_FOOTHOLD)
						m_Force = set_y_by_foothold(m_Force);
					//if (m_Box.GetBottomMiddle().x > -493.0f && m_Box.GetBottomMiddle().x < -485.0f)
					//	std::cout << "esddasdsad" << m_Foothold << std::endl;
					auto temp = checkIfNewFoothold(m_Force);
					if (temp != m_Foothold && temp != NO_FOOTHOLD)
					{
						//std::cout << "found next foothold old=" << m_Foothold << " new=" << temp << "and force="<<m_Force <<" mid="<< m_Box.GetBottomMiddle() << std::endl;
						m_Foothold = temp;
						m_Force = set_y_by_foothold(m_Force);
					}
					//else
					//	if (m_Foothold != NO_FOOTHOLD)
					//		m_Force = set_y_by_foothold(m_Force);
				}
				//if (temp != m_Foothold && temp != NO_FOOTHOLD)
				//{
				//	std::cout << "found next foothold old=" << m_Foothold << " new=" << temp << std::endl;
				//	m_Foothold = temp;
				//}
				//if (m_Foothold != NO_FOOTHOLD)
				//	m_Force = set_y_by_foothold(m_Force);
			}
			else
			{
				//m_Foothold = find_foothold_x(maths::vec2(0,m_Force.y));
				m_Foothold = find_foothold_x(m_Force);
				if (m_Foothold != NO_FOOTHOLD)
				{
					std::cout << "found foothold " << m_Foothold << std::endl;
					m_Force = set_y_by_foothold(m_Force);
				}
			}
			int foothold_vert = analyze_collision_y(m_Force);
			if (foothold_vert != NO_FOOTHOLD)
			{
				m_Force = force_by_vertical_foothold(m_Force, foothold_vert);
			}
			move(m_Force);
			if (m_Foothold != NO_FOOTHOLD)
			{
				m_Force.y = 0;
			}

		}

        bool Collisionable::check_if_still_on_foothold() const
        {
            const FootHold& foothold = m_MapLayer->getFootHolds().at(m_Foothold);
            return (m_Box.GetBottomMiddle().x > foothold.p1.x && m_Box.GetBottomMiddle().x < foothold.p2.x);
        }

		int Collisionable::checkIfNewFoothold(const maths::vec2& char_speed) const
		{
			const std::vector<physics::FootHold>& m_FootHolds = m_MapLayer->getFootHolds();
			const maths::vec2 char_pos = m_Box.GetBottomMiddle();
			//const maths::vec2 next_char_pos = (m_Box + maths::vec2{ char_speed.x , 0 }).GetBottomMiddle();
			const maths::vec2 next_char_pos = (m_Box + char_speed).GetBottomMiddle();

			for (int index = 0; index < m_FootHolds.size(); ++index)
			{
				const auto& foothold = m_FootHolds.at(index);
				if (index != m_Foothold)
					if (foothold.is_solid())
						if (foothold.LinesIntersect(char_pos, next_char_pos))
						{
							return index;
						}
			}
			return NO_FOOTHOLD;
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
                //d = (foothold.p1.x - m_Box.x) / (foothold.p1.x - foothold.p2.x);
                //d = (foothold.p1.x - m_Box.GetBottomMiddle().x) / (foothold.p1.x - foothold.p2.x);
                d = (foothold.p1.x - (m_Box.GetBottomMiddle().x + force.x)) / (foothold.p1.x - foothold.p2.x);
	        const float y_force = d * foothold.p2.y + (1 - d) * foothold.p1.y - m_Box.y;
            return {force.x, y_force};
        }
		maths::vec2 Collisionable::calc_force_by_foothold(const maths::vec2& force, int fh_id) const
		{
			physics::FootHold foothold = m_MapLayer->getFootHolds().at(fh_id);
			float d = 0;
			if (foothold.p1.x != foothold.p2.x)
				d = (foothold.p1.x - (m_Box.GetBottomMiddle().x + force.x)) / (foothold.p1.x - foothold.p2.x);
			const float y_force = d * foothold.p2.y + (1 - d) * foothold.p1.y - m_Box.y;
			return { force.x, y_force };
		}

        int Collisionable::find_foothold_x(const maths::vec2& char_speed) const
        {
			if (char_speed.y > 0)
				return NO_FOOTHOLD;

            const std::vector<physics::FootHold>& m_FootHolds = m_MapLayer->getFootHolds();
            const maths::vec2 char_pos = m_Box.GetBottomMiddle();
	        const maths::vec2 next_char_pos = (m_Box + char_speed).GetBottomMiddle();

			struct CalcDistStruct { int id; maths::vec2 rec_force; };
			std::vector<CalcDistStruct> save_vec;
			for (int index = 0; index < m_FootHolds.size(); ++index)
			{
				const auto& foothold = m_FootHolds.at(index);
				if (foothold.is_solid())
					if (foothold.LinesIntersect(char_pos, next_char_pos))
						//if (m_FootHolds->at(index).Intersects(next_char_rec) && !m_FootHolds->at(index).Intersects(char_rec)
					{
						save_vec.push_back({ index, calc_force_by_foothold(char_speed, index) });
						//return index;
					}
			}
			if (!save_vec.empty())
			{
				//std::cout << "size of list is " << save_vec.size() << std::endl;
				float  min_y = save_vec[0].rec_force.y;
				int  min_id = save_vec[0].id;
				for (const auto& dat : save_vec)
				{
				//std::cout << "my data is " << dat.id << " and " << dat.rec_force << std::endl;
					if (dat.rec_force.y > min_y)
						min_id = dat.id;
				}
				return min_id;
			}
            return NO_FOOTHOLD;
        }
        int Collisionable::find_next_foothold_x(const maths::vec2& char_speed) const
        {
			//if (char_speed.y > 0)
			//	return NO_FOOTHOLD;

            const std::vector<physics::FootHold>& m_FootHolds = m_MapLayer->getFootHolds();
            const maths::vec2 char_pos = m_Box.GetBottomMiddle();
	        const maths::vec2 next_char_pos = (m_Box + char_speed).GetBottomMiddle();

			for (int index = 0; index < m_FootHolds.size(); ++index)
			{
				const auto& foothold = m_FootHolds.at(index);
				if (foothold.is_solid())
					//if (foothold.Intersects({ char_pos, next_char_pos }))
					if (foothold.isOnFoothold(char_pos) || 
						foothold.LinesIntersect(char_pos, next_char_pos))
						//if (m_FootHolds->at(index).Intersects(next_char_rec) && !m_FootHolds->at(index).Intersects(char_rec)
					{
						return index;
					}
				//if (m_FootHolds.at(index).LinesIntersect(char_pos, next_char_pos))
				//        if (m_FootHolds.at(index).is_solid())
				//            //if (m_FootHolds->at(index).Intersects(next_char_rec) && !m_FootHolds->at(index).Intersects(char_rec)
				//        {
				//            return index;
				//        }
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