#pragma once

#include "graphics\renderable2d.h"
#include "graphics\texture.h"
#include "graphics\window.h"


namespace hiraeth {
	namespace map {
		class Map;
		class Portal : public graphics::Renderable2D
		{
		public:
			struct Serializer
			{
				maths::vec3 position;
				int next_map;
				template<class Archive>
				void serialize(Archive & ar)
				{
					ar(position, next_map);
				}
			};
		private:
			enum State {
				Default,
				Change_Map
			} m_State;
			//int m_Direction = 1;
			int m_FrameIndex, m_Counter;
			int m_NextMap;
			float m_FrameWidth;
			graphics::Window* m_Wnd;
			Map* m_Map;
		public:
			Portal(maths::vec3 position, float frame_width, int next_map, graphics::Window* wind, graphics::Texture* ptex, Map* map);
			Portal(Serializer s, float frame_width, graphics::Window* wind, graphics::Texture* ptex, Map* map)
				: Portal(s.position, frame_width, s.next_map, wind, ptex, map) {}

			void update() override;

		private:
			std::vector<maths::vec2> create_uv_by_pos_size(maths::vec2 pos, maths::vec2 size, maths::vec2 tex_size);
			void set_new_frame(int index);

		};
	}
}