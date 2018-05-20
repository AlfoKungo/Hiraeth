#include "ui_window.h"

namespace hiraeth {
	namespace ui {

		UiWindow::UiWindow(maths::Rectangle rec, input::Controls control_key)
			: m_ControlKey(control_key),
			m_WindowSize(rec.size),
			m_IsAttached(false),
			m_IsHolding(false),
			m_Group(rec.position)
		{
			m_Group.add(m_BackgroundGroup);
			m_Group.add(m_ForegroundGroup);
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

		bool UiWindow::isWindowContains(float x, float y) const
		{
			maths::vec2 rPos(getRelativeLocation(x, y));
			if (maths::Rectangle(0, m_WindowSize).Contains(rPos))
				return true;
			return false;
		}

		bool UiWindow::isTitlebarContains(float x, float y) const
		{
			maths::vec2 rPos(getRelativeLocation(x, y));
			maths::Rectangle rec = getTitlebar();
			if (rec.Contains(rPos))
				return true;
			return false;
		}

		void UiWindow::mouse_clicked_full(maths::vec2 mousePos)
		{
			for (auto & value : m_Buttons) 
			{
				value->onClick(mousePos);
			}
			mouse_clicked(mousePos);
		}

		void UiWindow::mouse_released_full(maths::vec2 mousePos)
		{
			maths::vec2 relMousePos(getRelativeLocation(mousePos.x, mousePos.y));
			for (auto & value : m_Buttons) 
			{
				value->onRelease(relMousePos);
			}
			//if (is_holding())
				mouse_released(relMousePos);
		}

		void UiWindow::mouse_moved_full(maths::vec2 mousePos, maths::vec2 pMousePos)
		{
			for (auto & value : m_Buttons) 
			{
				value->onMove(mousePos);
			}
			maths::vec2 mouseMove(pMousePos - mousePos);
			if (is_holding())
				mouse_moved(mouseMove.x, -mouseMove.y);
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
