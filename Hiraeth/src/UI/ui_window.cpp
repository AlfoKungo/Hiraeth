#include "ui_window.h"

namespace hiraeth {
	namespace ui {

		UiWindow::UiWindow(maths::Rectangle rec, UiKey control_key)
			: m_ControlKey(control_key),
			m_WindowSize(rec.size),
			m_IsAttached(false),
			m_IsHolding(false),
			m_Group(rec.position)
		{
			m_ForegroundGroup = new graphics::Group();
			m_BackgroundGroup = new graphics::Group();
			m_Buttons = new graphics::TGroup<ui::UiButton>();
			m_Group.add(m_BackgroundGroup);
			m_Group.add(m_ForegroundGroup);
			m_Group.add(m_Buttons);
		}

		void UiWindow::controlKeyClicked()
		{
			is_to_draw = !is_to_draw;
			is_to_update = !is_to_update;
		}

		void UiWindow::move(float mx, float my)
		{
			m_Group.translate(maths::vec2(-mx, my));
		}

		bool UiWindow::isWindowContains(maths::vec2 pos) const
		{
			maths::vec2 rPos(getRelativeLocation(pos));
			return maths::Rectangle(0, m_WindowSize).Contains(rPos);
		}

		bool UiWindow::isTitlebarContains(float x, float y) const
		{
			maths::vec2 rPos(getRelativeLocation(x, y));
			maths::Rectangle rec = getTitlebar();
			return rec.Contains(rPos);
		}

		void UiWindow::mouse_left_clicked_full(maths::vec2 mousePos)
		{
			for (auto & value : m_Buttons->m_Renderables) 
			{
				value->onClick(mousePos);
			}
			mouse_left_clicked(mousePos);
		}

		void UiWindow::mouse_left_released_full(maths::vec2 mousePos)
		{
			maths::vec2 relMousePos(getRelativeLocation(mousePos.x, mousePos.y));
			for (auto & value : m_Buttons->m_Renderables)
			{
				value->onRelease(relMousePos);
			}
			mouse_left_released(relMousePos);
		}

		void UiWindow::mouse_right_clicked_full(maths::vec2 mousePos)
		{
			maths::vec2 relMousePos(getRelativeLocation(mousePos.x, mousePos.y));
			mouse_right_clicked(relMousePos);
		}

		void UiWindow::mouse_moved_full(maths::vec2 mousePos, maths::vec2 pMousePos)
		{
			for (auto & value : m_Buttons->m_Renderables)
			{
				value->onMove(mousePos);
			}
			maths::vec2 mouseMove(pMousePos - mousePos);
			mouse_moved(mouseMove.x, -mouseMove.y, mousePos);
		}

		void UiWindow::add_button(UiButton * new_button)
		{
			m_Buttons->add(new_button);
		}

		maths::vec2 UiWindow::getRelativeLocation(float mx, float my) const
		{
			return getRelativeLocation(maths::vec2(mx, my));
		}

		maths::vec2 UiWindow::getRelativeLocation(maths::vec2 mouse_pos) const
		{
			maths::mat4 transform = m_Group.getTransform();
			maths::Rectangle rec = maths::Rectangle(0, m_WindowSize).Transoform(transform);
			maths::vec2 relPos = maths::vec2(mouse_pos.x - 800, 450 - mouse_pos.y) - rec.position;
			return relPos;
		}

		maths::Rectangle UiWindow::getTitlebar() const
		{
			return maths::Rectangle(0, m_WindowSize.y - TITLE_BAR_SIZE, m_WindowSize.x, TITLE_BAR_SIZE);
		}



	}
}
