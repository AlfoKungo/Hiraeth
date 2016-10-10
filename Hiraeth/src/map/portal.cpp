#include "portal.h"
#include "map.h"

namespace hiraeth {
	namespace map {

		Portal::Portal(maths::vec3 position, float frame_width, int next_map, graphics::Window* wind, graphics::Texture* ptex, Map* map)
			: Renderable2D(position, maths::vec2(frame_width, ptex->getHeight())),
			//: Renderable2D(position, maths::vec2(ptex->getWidth(), ptex->getHeight())),
			m_Wnd(wind), m_Map(map), m_State(State::Change_Map), m_FrameIndex(0), m_FrameWidth(frame_width)
		{
			m_Texture = ptex;
			m_NextMap = next_map;
			set_new_frame(m_FrameIndex);
		}

		void Portal::update()
		{
 			if (m_State == State::Default && m_Wnd->isKeyPressed(GLFW_KEY_SPACE))
			{
				m_Map->change_map(m_NextMap);
				m_NextMap = m_NextMap ? 0 : 1;
				m_State = State::Change_Map;
			}
			else if (m_State == State::Change_Map && !m_Wnd->isKeyPressed(GLFW_KEY_SPACE))
			{
				m_State = State::Default;
			}
			m_Counter++;
			if (m_Counter % 80 == 0)
			{
				set_new_frame(m_FrameIndex);
				//std::cout << m_FrameIndex << std::endl;
				//m_FrameIndex += m_Direction;
				//if (m_FrameIndex == 0 || m_FrameIndex == 7)
				//{
				//	m_Direction *= -1;
				//}
				m_FrameIndex++;
				m_FrameIndex = m_FrameIndex % 8;
				m_Counter = 0;
			}
		}

		void Portal::set_new_frame(int index)
		{
			m_UV = create_uv_by_pos_size(maths::vec2(index * m_FrameWidth, 0) , maths::vec2(m_FrameWidth, 150), maths::vec2(m_Texture->getWidth(), m_Texture->getHeight()));
		}
		
		std::vector<maths::vec2> Portal::create_uv_by_pos_size(maths::vec2 pos, maths::vec2 size, maths::vec2 tex_size)
		{
			std::vector<maths::vec2> uv;
			uv.push_back((pos) / tex_size);
			uv.push_back((pos + maths::vec2(0, size.y)) / tex_size);
			uv.push_back((pos + maths::vec2(size.x, size.y)) / tex_size);
			uv.push_back((pos + maths::vec2(size.x, 0)) / tex_size);
			return uv;
		}
	}
}