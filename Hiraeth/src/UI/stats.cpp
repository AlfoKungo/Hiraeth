#include "stats.h"

namespace hiraeth {
	namespace ui {

		Stats::Stats(maths::vec2 pos, Timer* time)
			: UiWindow(maths::Rectangle(pos.x, pos.y, 212, 373)),
			m_Layer(new graphics::Shader("src/shaders/basic.vert", "src/shaders/basic.frag")),
			m_StatsStruct{ "Kanye", "Creative Genius", 43, 4039204, 4, 45,
						   "Good Music", 4300, 2000, 9999, 3, 4, 9, 9, 4 }
		{
			m_Group.add(new graphics::Sprite(0, 0, graphics::TextureManager::Load("Stat.main.backgrnd.png")));
			m_Group.add(new graphics::Sprite(6, 5, graphics::TextureManager::Load("Stat.main.backgrnd2.png")));
			m_Group.add(new graphics::Sprite(7, 114, graphics::TextureManager::Load("Stat.main.backgrnd3.png")));
			//m_Group.add(new graphics::Sprite(120, 120, graphics::TextureManager::Load("Stat.main.BtAuto.normal.1.png")));
			m_Group.add(new graphics::SpritedRenderable(maths::vec3(120, 120, 0), 4, 0.15f, true, graphics::TextureManager::Load("Stat.main.BtAuto.normal.png"), time));
			m_Group.add(new graphics::Label("arial", 13, std::to_string(m_StatsStruct.Luk), 74, 44, 0xff000000));
			m_Group.add(new graphics::Label("arial", 13, std::to_string(m_StatsStruct.Int), 74, 62, 0xff000000));
			m_Group.add(new graphics::Label("arial", 13, std::to_string(m_StatsStruct.Dex), 74, 80, 0xff000000));
			m_Group.add(new graphics::Label("arial", 13, std::to_string(m_StatsStruct.Str), 74, 98, 0xff000000));
			m_Group.add(new graphics::Label("arial", 13, std::to_string(m_StatsStruct.AbilityPoints), 78, 124, 0xff000000));
			m_Group.add(new graphics::Label("arial", 13, std::to_string(m_StatsStruct.Fame), 74, 167, 0xff000000));
			m_Group.add(new graphics::Label("arial", 13, std::to_string(m_StatsStruct.Mp), 74, 185, 0xff000000));
			m_Group.add(new graphics::Label("arial", 13, std::to_string(m_StatsStruct.Hp), 74, 203, 0xff000000));
			m_Group.add(new graphics::Label("arial", 13, m_StatsStruct.Guild, 74, 221, 0xff000000));
			m_Group.add(new graphics::Label("arial", 13, std::to_string(m_StatsStruct.HonorExp), 74, 239, 0xff000000));
			m_Group.add(new graphics::Label("arial", 13, std::to_string(m_StatsStruct.HonorLvl), 74, 257, 0xff000000));
			m_Group.add(new graphics::Label("arial", 13, std::to_string(m_StatsStruct.Exp), 74, 275, 0xff000000));
			m_Group.add(new graphics::Label("arial", 13, std::to_string(m_StatsStruct.Level), 74, 293, 0xff000000));
			m_Group.add(new graphics::Label("arial", 13, m_StatsStruct.Job, 74, 311, 0xff000000));
			m_Group.add(new graphics::Label("arial", 13, m_StatsStruct.Name, 74, 329, 0xff000000));
			m_Layer.add(&m_Group);
		}
		
		void Stats::draw()
		{
			m_Layer.render();
		}
	
		void Stats::update()
		{
			m_Group.update();
		}
	}
}
