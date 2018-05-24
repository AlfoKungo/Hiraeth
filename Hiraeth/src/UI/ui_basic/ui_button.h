#pragma once
#include "basic/drawable.h"
#include "basic/updatable.h"
#include "graphics/layers/group.h"
#include "graphics/renderable.h"

namespace hiraeth {
	namespace ui {

		class UiButton : public graphics::Renderable
		{
		protected:
			graphics::Renderable * m_Rend;
			std::function<void()> m_FuncToCall;
		public:
			UiButton(graphics::Renderable * renderable, std::function<void()> func_to_call)
				: m_Rend(renderable),
				m_FuncToCall(func_to_call)
			{
			}

			~UiButton() {}

			UiButton(const UiButton&) = default;
			UiButton& operator=(const UiButton&) = default;
			UiButton(UiButton && o)  noexcept : m_Rend(o.m_Rend), m_FuncToCall(o.m_FuncToCall) {}

			bool isContains(maths::vec2 mouse_pos)
			{
				return m_Rend->getBounds().Contains(mouse_pos);
			}

			virtual void update() override
			{
				m_Rend->update();
			}
			virtual void draw(graphics::Renderer* renderer) const override { m_Rend->draw(renderer); }


			virtual void onClick(maths::vec2 mouse_pos)
			{
				if (isContains(mouse_pos))
					m_FuncToCall();
			}
			virtual void onRelease(maths::vec2 mouse_pos) {}
			virtual void onMove(maths::vec2 mouse_pos) {}
		};
	}
}