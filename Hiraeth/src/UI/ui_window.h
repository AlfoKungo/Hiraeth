#pragma once
#include "keyboard\keyboard_event.h"
#include "keyboard\mouse_event.h"
#include "graphics\layers\group.h"
#include "basic\drawable.h"
#include "basic\updatable.h"

namespace hiraeth {
	namespace ui {

		class UiWindow : public input::KeyboardEvent, public Drawable, public Updatable
		{
		protected:
			graphics::Group m_Group;
			maths::vec2 m_WindowSize;
			bool m_IsAttached;
		public:
			//UiWindow() : m_Group(maths::mat4::Translate(maths::vec3(-400, -50, 0))), 
			UiWindow(maths::Rectangle rec)
			 : m_Group(maths::mat4::Translate(rec.position)), 
				m_WindowSize(rec.size),
				m_IsAttached(false)
			{}
			void ButtonClicked(input::Controls c) override
			{
				is_to_draw = !is_to_draw;
				is_to_update = !is_to_update;
			}
			void ButtonReleased(input::Controls c) override {}
			void move(float mx, float my)
			{
				m_Group.translate(maths::vec2(-mx, my));
			}
			bool isWindowContains(float x, float y)
			{
				maths::mat4 t = m_Group.getTransform();
				maths::Rectangle r = maths::Rectangle(t * maths::vec2(0), m_WindowSize);
				maths::vec2 n = maths::vec2(x - 800, -(y - 450));
				if (r.Contains(n))
					return true;
				return false;
			}
			bool isTitlebarContains(float x, float y)
			{
				maths::mat4 t = m_Group.getTransform();
				maths::Rectangle r = getTitlebar();
				r.position = t * r.position;
				maths::vec2 n = maths::vec2(x - 800, -(y - 450));
				if (r.Contains(n))
					return true;
				return false;
			}
			void attach() { m_IsAttached = true; }
			void unattach() { m_IsAttached = false; }
			bool is_attached() { return m_IsAttached; }
			virtual void mouse_clicked() = 0;
			virtual void mouse_released() = 0;
			virtual void mouse_moved(float mx, float my) = 0;
		private:
			maths::Rectangle getTitlebar() 
			{ 
				return maths::Rectangle(0, m_WindowSize.y - 25, m_WindowSize.x, 25); 
			}
		};

	}
}