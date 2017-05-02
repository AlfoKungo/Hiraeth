#pragma once

#include "graphics\spritedrenderable.h"
#include "keyboard\keyboard.h"
#include "map\maplayer.h"
#include "graphics\texturemanager.h"
#include "physics\collisionable.h"

namespace hiraeth {
	namespace game {

		struct CreatureControls
		{
			bool left = false;
			bool right = false;
			bool up = false;
			bool down = false;
			bool jump = false;
		};

		class Creature : public graphics::Renderable2D, public physics::Collisionable
		{

#define CHARACTER_SPEED 25.0f
#define CHARACTER_JUMP 12.0f
#define CHARACTER_GRAVITY 0.5f;
#define FRICTION maths::vec2(0.2f, 1.0f)
#define CHARACTER_TIME_BETWEEN_ADDS 1.0f/60.0f
#define FORCE_OF_GRAVITY 0.94f

		enum StanceState {
			Stand,
			Walk,
		} m_StanceState;
		enum Direction {
			Right,
			Left,
		} m_Direction;

		protected:
			CreatureControls m_Controls;

		private:
			std::vector<int> actions; //this is to create a list of actions to do.
			std::vector<graphics::SpritedRenderable*> m_StandRenderables;
			std::vector<graphics::SpritedRenderable*> m_WalkRenderables;
			//map::MapLayer* m_MapLayer;
			float m_MovementTimer;
			maths::vec2 m_Force;
			maths::mat4 m_TransformationMatrix;
			Timer* m_Time;
		public:
			Creature(maths::vec3 pos, Timer* time, input::Keyboard* kb,
				map::MapLayer* m_MapLayer);
			virtual ~Creature();

			void update() override;
			void submit(graphics::Renderer2D* renderable) const override;
			void move_to(const maths::vec2& pos);
		private:
			//physics::CollisionStruct analyze_collision(maths::Rectangle char_rec, maths::vec2 char_speed);
			//void set_foothold(int foothold);
			//bool check_if_still_on_foothold();
			//void set_y_by_foothold();
			void analyze_controls();
			void change_stance(StanceState next_state);
			void add_gravity(float* y_force) { *y_force -= FORCE_OF_GRAVITY; }
			virtual CreatureControls set_update_controls() = 0;

			//maths::vec3 getPosition() override { return m_TransformationMatrix.; }
		};
	}
}