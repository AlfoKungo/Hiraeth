#pragma once
#include "keyboard/keyboard_event.h"
#include "keyboard/mouse_event.h"
#include "graphics/layers/group.h"
#include "graphics/layers/layer.h"
#include "basic/drawable.h"
#include "basic/updatable.h"
#include "keyboard/keyboard.h"
#include "ui_basic/ui_button.h"

namespace hiraeth {
	namespace ui {
#define TITLE_BAR_SIZE 25

		class UiWindow : public Drawable, public Updatable
		{
		private:
			input::Controls m_ControlKey;
			graphics::Group m_Group;
		protected:
			maths::vec2 m_WindowSize;
			bool m_IsAttached, m_IsHolding;
			graphics::Group m_ForegroundGroup;
			graphics::Group m_BackgroundGroup;
			std::vector<std::unique_ptr<ui::UiButton>> m_Buttons;
		public:
			UiWindow(maths::Rectangle rec, input::Controls control_key);

			void controlKeyClicked();

			void move(float mx, float my);

			bool isWindowContains(float x, float y) const;
			bool isTitlebarContains(float x, float y) const;

			void attach() { m_IsAttached = true; }
			void unattach() { m_IsAttached = false; }
			inline bool is_attached() const { return m_IsAttached; }
			inline bool is_holding() const { return m_IsHolding; }
			inline input::Controls getControlKey() const { return m_ControlKey; }

			virtual void mouse_clicked_full(maths::vec2 mousePos);
			virtual void mouse_released_full(maths::vec2 mousePos);
			virtual void mouse_moved_full(maths::vec2 mousePos, maths::vec2 pMousePos);


			virtual void mouse_clicked(maths::vec2 mousePos) = 0;
			virtual void mouse_released(maths::vec2 mousePos) = 0;
			virtual void mouse_moved(float mx, float my) = 0;

			void update() override { m_Group.update(); }
			void draw(graphics::Renderer* renderer) const override { m_Group.draw(renderer); }
		//protected:
			maths::vec2 getRelativeLocation(maths::vec2 mouse_pos) const;
			maths::vec2 getRelativeLocation(float mx, float my) const;
		private:
			maths::Rectangle getTitlebar() const;
		};

	}
}