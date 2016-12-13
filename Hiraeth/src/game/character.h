#pragma once

#include "graphics\spritedrenderable.h"
#include "graphics\window.h"
#include "map\maplayer.h"
#include "graphics\texturemanager.h"

namespace hiraeth {
	namespace game {

		class Character : public graphics::Renderable2D
		{


#define CHARACTER_SPEED 0.5f
#define CHARACTER_GRAVITY 0.5f;
#define FRICTION 0.4f
#define CHARACTER_TIME_BETWEEN_ADDS 0.001f
#define NO_FOOTHOLD -1

		private:
			enum StanceState {
				Stand,
				Walk,
			} m_StanceState;
			enum Direction {
				Right,
				Left,
			} m_Direction;
			std::vector<graphics::SpritedRenderable*> m_StandRenderables;
			std::vector<graphics::SpritedRenderable*> m_WalkRenderables;
			int m_Foothold;
			graphics::Window* m_Wnd;
			map::MapLayer* m_MapLayer;
			float m_MovementTimer;
			maths::vec2 m_Force;
			maths::mat4 m_TransformationMatrix;
			Timer* m_Time;
		public:
			Character(maths::vec3 pos, Timer* time, graphics::Window* m_Wnd, 
				map::MapLayer* m_MapLayer);
			~Character();

			void update() override;
			void submit(graphics::Renderer2D* renderable) const override;
			inline void move_to(const maths::vec2& pos);
		private:
			//maths::vec2 analyze_collision(maths::Rectangle char_rec, maths::vec2 char_speed);
			int analyze_collision(maths::Rectangle char_rec, maths::vec2 char_speed);
			void set_foothold(int foothold);
			bool check_if_still_on_foothold();
			void set_y_by_foothold();
			void change_stance(StanceState next_state);
			//maths::vec3 getPosition() override { return m_TransformationMatrix.; }
		};
	}
}