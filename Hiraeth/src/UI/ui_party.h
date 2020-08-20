#pragma once
#include "ui_window.h"
#include "graphics/texture_manager.h"
#include "graphics/sprite.h"
#include "graphics/label.h"
#include "net/protocol.h"

namespace hiraeth {
	namespace ui {

		class UiParty : public UiWindow
		{
		private:
			graphics::Label m_PartyMemebersLabel;
		public:
			UiParty(maths::vec2 pos, UiKey control_key);
			void mouse_left_clicked(maths::vec2 mousePos) override {}
			void mouse_left_released(maths::vec2 mousePos) override {}
			void mouse_right_clicked(maths::vec2 mousePos) override {}
			void mouse_moved(float mx, float my, maths::vec2 mousePos) override {}

			void set_party(decltype(network::PartyUpdateMsg::party_members) party_members)
			{
				std::string names="";
				for (const auto& [id, name] : party_members)
				{
					names += name + "\n";
				}
				m_PartyMemebersLabel.setText(names);
			}
		};
	}
}
