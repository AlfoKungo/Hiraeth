#pragma once
#include "keyboard/keyboard_event.h"
#include "keyboard/mouse_event.h"
#include "graphics/layers/group.h"
#include "graphics/layers/layer.h"
#include "basic/drawable.h"
#include "basic/updatable.h"
#include "keyboard/keyboard.h"
#include "ui_basic/ui_button.h"
#include "graphics/layers/t_group.h"

namespace hiraeth {
	namespace ui {
#define TITLE_BAR_SIZE 25

		enum UiKey
		{
			escape,
			stats,
			skills,
			quests,
			equip,
			inventory,
		};
		class UiWindow : public Drawable, public Updatable
		{
		private:
			UiKey m_ControlKey;
		protected:
			graphics::Group m_Group;
			maths::vec2 m_WindowSize;
			bool m_IsAttached, m_IsHolding;
			graphics::Group * m_ForegroundGroup;
			graphics::Group * m_BackgroundGroup;
			graphics::TGroup<ui::UiButton> * m_Buttons;
		public:
			UiWindow(maths::Rectangle rec, UiKey control_key);

			void controlKeyClicked();

			void move(float mx, float my);

			bool isWindowContains(maths::vec2 pos) const;
			bool isTitlebarContains(float x, float y) const;

			void attach() { m_IsAttached = true; }
			void unattach() { m_IsAttached = false; }
			inline bool is_attached() const { return m_IsAttached; }
			inline bool is_holding() const { return m_IsHolding; }
			inline UiKey getControlKey() const { return m_ControlKey; }

			virtual void mouse_left_clicked_full(maths::vec2 mousePos);
			virtual void mouse_left_released_full(maths::vec2 mousePos);
			virtual void mouse_right_clicked_full(maths::vec2 mousePos);
			virtual void mouse_moved_full(maths::vec2 mousePos, maths::vec2 pMousePos);


			virtual void mouse_left_clicked(maths::vec2 mousePos) = 0;
			virtual void mouse_left_released(maths::vec2 mousePos) = 0;
			virtual void mouse_right_clicked(maths::vec2 mousePos) = 0;
			virtual void mouse_moved(float mx, float my, maths::vec2 mousePos) = 0;

			void update() override { m_Group.update(); }
			void draw(graphics::Renderer* renderer) const override 
			{ 
				m_Group.draw(renderer); 
			}
		//protected:
			maths::vec2 getRelativeLocation(maths::vec2 mouse_pos) const;
			maths::vec2 getRelativeLocation(float mx, float my) const;
		protected:
			void add_button(UiButton * new_button);
		private:
			maths::Rectangle getTitlebar() const;
		};

	}
}