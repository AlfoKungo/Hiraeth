#pragma once
#include "keyboard/keyboard_event.h"
#include "keyboard/mouse_event.h"
#include "graphics/layers/group.h"
#include "graphics/layers/layer.h"
#include "basic/drawable.h"
#include "basic/updatable.h"
#include "keyboard/keyboard.h"

namespace hiraeth {
	namespace ui {
#define TITLE_BAR_SIZE 25

		class UiWindow : public Drawable, public Updatable
		{
		private:
			input::Controls m_ControlKey;
		protected:
			maths::vec2 m_WindowSize;
			bool m_IsAttached;
			graphics::Group m_Group;
		public:
			UiWindow(maths::Rectangle rec, input::Controls control_key)
				: m_ControlKey(control_key),
				m_WindowSize(rec.size),
				m_IsAttached(false),
				m_Group(rec.position)
			{

			}

			void controlKeyClicked()
			{
				is_to_draw = !is_to_draw;
				is_to_update = !is_to_update;
			}

			void move(float mx, float my)
			{
				m_Group.translate(maths::vec2(-mx, my));
			}

			bool isWindowContains(float x, float y) const
			{
				maths::mat4 transform = m_Group.getTransform();
				maths::Rectangle rec = maths::Rectangle(0, m_WindowSize).Transoform(transform);
				maths::vec2 cursorPos = maths::vec2(x - 800, -(y - 450));
				if (rec.Contains(cursorPos))
					return true;
				return false;
			}
			bool isTitlebarContains(float x, float y) const
			{
				maths::mat4 transform = m_Group.getTransform();
				maths::Rectangle rec = getTitlebar().Transoform(transform);
				maths::vec2 cursorPos = maths::vec2(x - 800, -(y - 450));
				if (rec.Contains(cursorPos))
					return true;
				return false;
			}
			void attach() { m_IsAttached = true; }
			void unattach() { m_IsAttached = false; }
			inline bool is_attached() const { return m_IsAttached; }
			inline input::Controls getControlKey() const { return m_ControlKey; }

			virtual void mouse_clicked() = 0;
			virtual void mouse_released() = 0;
			virtual void mouse_moved(float mx, float my) = 0;
			void update() override { m_Group.update(); }
			void draw(graphics::Renderer2D* renderer) const override { m_Group.draw(renderer); }
		private:
			maths::Rectangle getTitlebar() const
			{ 
				return maths::Rectangle(0, m_WindowSize.y - TITLE_BAR_SIZE, m_WindowSize.x, TITLE_BAR_SIZE); 
			}
		};

	}
}