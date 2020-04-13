#pragma once
#include "renderable.h"
#include "layers/group.h"
#include "sprite.h"

namespace hiraeth {
	namespace graphics {

		class BoxTextured : public Renderable
		{
			Group m_Group;
			Sprite m_Top, m_Mid, m_Bot;
			BoxTextured(maths::vec2 pos, int size, std::string path)
				: m_Top{ 0 },
			m_Mid{0},
			m_Bot{0}
			{
				m_Group.add(&m_Top);
				m_Group.add(&m_Mid);
				m_Group.add(&m_Bot);
			}
			void draw(Renderer* renderer) const override
			{
				m_Group.draw(renderer);
			}
			void update() override
			{
				m_Group.update();
			}
			void setSize(int size)
			{
				m_Mid.setSize({ m_Mid.getSize().x, size });
			}
		};
	}
}
