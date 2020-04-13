#pragma once
#include "renderable.h"
#include "layers/group.h"
#include "sprite.h"
#include "texture_manager.h"

namespace hiraeth {
	namespace graphics {

		class BoxTexturedFull : public Renderable
		{
			Group m_Group;
			maths::vec2 m_Pos, m_Size;
			Sprite m_N, m_E, m_S, m_W;
			Sprite m_NE, m_SE, m_SW, m_NW;
			Sprite m_C, m_A;
			maths::vec2 m_NEs{ 6,6 }, m_SEs{ 6,6 }, m_SWs{ 6,6 }, m_NWs{ 6,6 };
			//maths::vec2 m_Ns{ 12,6 }, m_Es{ 6,14 }, m_Ss{ 12,6 }, m_Ws{ 6,14 };
			float m_Nb, m_Eb, m_Sb, m_Wb;
			maths::vec2 m_Ns, m_Es, m_Ss, m_Ws;
		public:
			BoxTexturedFull(maths::vec2 pos, maths::vec2 size, std::string path)
				: m_Pos{ pos }, m_Size(size),
				m_N{ pos, TextureManager::Load(path + ".n.png") },
				m_E{ pos, TextureManager::Load(path + ".e.png") },
				m_S{ pos, TextureManager::Load(path + ".s.png") },
				m_W{ pos, TextureManager::Load(path + ".w.png") },
				m_NE{ pos, TextureManager::Load(path + ".ne.png") },
				m_SE{ pos, TextureManager::Load(path + ".se.png") },
				m_SW{ pos, TextureManager::Load(path + ".sw.png") },
				m_NW{ pos, TextureManager::Load(path + ".nw.png") },
				m_C{ pos, TextureManager::Load(path + ".c.png") },
				m_A{ pos, TextureManager::Load(path + ".arrow.png") }
			{
				m_Group.add_ref(&m_N);
				m_Group.add_ref(&m_E);
				m_Group.add_ref(&m_S);
				m_Group.add_ref(&m_W);
				m_Group.add_ref(&m_NE);
				m_Group.add_ref(&m_SE);
				m_Group.add_ref(&m_SW);
				m_Group.add_ref(&m_NW);
				m_Group.add_ref(&m_C);
				m_Group.add_ref(&m_A);
				m_Ns = m_N.getSize();
				m_Es = m_E.getSize();
				m_Ss = m_S.getSize();
				m_Ws = m_W.getSize();
				m_Nb = m_N.getSize().y;
				m_Eb = m_E.getSize().x;
				m_Sb = m_S.getSize().y;
				m_Wb = m_W.getSize().x;
				setSize(size);
			}
			void draw(Renderer* renderer) const override
			{
				m_Group.draw(renderer);
			}
			void update() override
			{
				m_Group.update();
			}
			void setSize(maths::vec2 size)
			{
				m_Size = size;
				m_N.setPosition(m_Pos + maths::vec2{  - size.x / 2, size.y  });
				m_E.setPosition(m_Pos + maths::vec2{ size.x / 2  ,0 });
				m_S.setPosition(m_Pos + maths::vec2{ -size.x / 2, -m_Ss.y });
				m_W.setPosition(m_Pos + maths::vec2{ -size.x / 2 - m_Ws.x,0 });
				m_C.setPosition(m_Pos + maths::vec2{  - size.x/2,0 });
				m_A.setPosition(m_Pos + maths::vec2{ 0,-m_A.getSize().y });
				m_NE.setPosition(m_Pos + maths::vec2{ size.x / 2, size.y  });
				m_SE.setPosition(m_Pos + maths::vec2{ size.x / 2,-m_SE.getSize().y });
				m_SW.setPosition(m_Pos + maths::vec2{ -size.x / 2, 0 } - m_SW.getSize());
				m_NW.setPosition(m_Pos + maths::vec2{ -size.x / 2 - m_NW.getSize().x ,size.y  });

				m_N.setSize(maths::vec2{ size.x , m_N.getSize().y });
				m_E.setSize(maths::vec2{ m_E.getSize().x,size.y  });
				m_S.setSize(maths::vec2{ size.x, m_S.getSize().y });
				m_W.setSize(maths::vec2{ m_W.getSize().x,size.y  });
				m_C.setSize(size);

				float repx = float(size.x ) / m_Ss.x;
				m_S.setUv(std::vector<maths::vec2>{{0, 0}, {0,1}, {repx,1}, {repx,0}});
				m_N.setUv(std::vector<maths::vec2>{{0, 0}, {0,1}, {repx,1}, {repx,0}});
				float repy = float(size.y ) / m_Es.y;
				m_W.setUv(std::vector<maths::vec2>{{0, 0}, {0,repy}, {1,repy}, {1,0}});
				m_E.setUv(std::vector<maths::vec2>{{0, 0}, {0,repy}, {1,repy}, {1,0}});
			}
			//void setSize(maths::vec2 size)
			//{
			//	m_Size = size;
			//	m_N.setPosition(m_Pos + maths::vec2{ m_Wb - size.x / 2, size.y - m_Sb });
			//	m_E.setPosition(m_Pos + maths::vec2{ size.x / 2 - m_Wb,m_Sb });
			//	m_S.setPosition(m_Pos + maths::vec2{ m_Wb - size.x / 2, 0 });
			//	m_W.setPosition(m_Pos + maths::vec2{ -size.x / 2,m_Sb });
			//	//m_C.setPosition(m_Pos + maths::vec2{ m_Wb - size.x/2,m_Sb });
			//	m_C.setPosition(m_Pos + maths::vec2{  - size.x/2,m_Sb });
			//	m_A.setPosition(m_Pos + maths::vec2{ 0,-m_A.getSize().y + m_Sb});
			//	m_NE.setPosition(m_Pos + maths::vec2{ size.x / 2- m_Wb, size.y - m_Sb });
			//	m_SE.setPosition(m_Pos + maths::vec2{ size.x / 2- m_Wb,0 });
			//	m_SW.setPosition(m_Pos + maths::vec2{ -size.x / 2- (m_SW.getSize().x - m_Wb), 0 });
			//	m_NW.setPosition(m_Pos + maths::vec2{ -size.x / 2 - (m_NW.getSize().x - m_Wb),size.y - m_Sb });

			//	m_N.setSize(maths::vec2{ size.x - 12, m_N.getSize().y });
			//	m_E.setSize(maths::vec2{ m_E.getSize().x,size.y - 12 });
			//	m_S.setSize(maths::vec2{ size.x - 12, m_S.getSize().y });
			//	m_W.setSize(maths::vec2{ m_W.getSize().x,size.y - 12 });
			//	//m_C.setSize(size - maths::vec2{ m_Wb + m_Eb, m_Sb + m_Nb } );
			//	m_C.setSize(size);

			//	float repx = float(size.x - m_Wb - m_Eb) / m_Ss.x;
			//	m_S.setUv(std::vector<maths::vec2>{{0, 0}, {0,1}, {repx,1}, {repx,0}});
			//	m_N.setUv(std::vector<maths::vec2>{{0, 0}, {0,1}, {repx,1}, {repx,0}});
			//	float repy = float(size.y - m_Nb - m_Sb) / m_Es.y;
			//	m_W.setUv(std::vector<maths::vec2>{{0, 0}, {0,repy}, {1,repy}, {1,0}});
			//	m_E.setUv(std::vector<maths::vec2>{{0, 0}, {0,repy}, {1,repy}, {1,0}});
			//}
		};
	}
}
