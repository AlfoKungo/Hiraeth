#pragma once
#include "graphics/label.h"
#include "graphics/layers/layer.h"

namespace hiraeth {
	namespace npc {

		class IDialogInitiator
		{
			unsigned int dialogReturnValue();
		};

		class DialogTree
		{
		private:
			std::vector<std::string> m_Strings;
			size_t m_StringIndex;
		public:
			DialogTree() 
				: m_Strings{"This is the first text", "this is seconds", "and then i die"},
				m_StringIndex(0)
			{
				
			}

			std::string getNextText()
			{
				if (m_Strings.size() > m_StringIndex)
					return m_Strings[m_StringIndex++];
				return "";
			}
		};

		class DialogManager : Updatable
		{
		private:
			graphics::Layer<graphics::Renderable> m_TextBoxes;
			graphics::Label* m_InText;
			bool m_IsDraw{false};
			DialogTree m_DialogTree;

		public:
			DialogManager()
				: m_TextBoxes(new graphics::Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag"), false),
			m_InText{nullptr},
			m_DialogTree{}
			{
				m_InText = new graphics::Label{ "arial", 16, "", { 40, 90 }, 0xffffffff, graphics::Label::Alignment::LEFT };
				m_TextBoxes.add(m_InText);
				m_TextBoxes.add(new graphics::Sprite{ maths::vec2(30, 75), 180, 80, 0x88331a00 });
			}

			void createDialog(const std::string& text)
			{
				m_IsDraw = true;
				m_InText->setText(text);
			}

			void nextDialog()
			{
				m_IsDraw = true;
				const std::string next_text{ m_DialogTree.getNextText() };
				if (!next_text.empty())
					m_InText->setText(next_text);
				else
					m_TextBoxes.clear();
			}

			void draw() const
			{
				if (m_IsDraw)
					m_TextBoxes.render();
			}
			void update() override
			{
				//m_TextBoxes.
			}
		};
	}
}
