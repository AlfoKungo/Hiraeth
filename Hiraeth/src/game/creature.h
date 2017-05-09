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

		enum StanceState {
			Stand,
			Walk,
		};
		enum Direction {
			Right,
			Left,
		};

		class Creature : public graphics::Renderable2D, public physics::Collisionable
		{

#define CHARACTER_SPEED 15.0f
#define CHARACTER_JUMP 12.0f
#define CHARACTER_GRAVITY 0.5f;
#define FRICTION maths::vec2(0.2f, 1.0f)
#define FRICTION_AIR maths::vec2(0.99f, 1.0f)
#define CHARACTER_TIME_BETWEEN_ADDS 1.0f/60.0f
#define FORCE_OF_GRAVITY 0.94f

		protected:
			StanceState m_StanceState;
			Direction m_Direction;
			CreatureControls m_Controls;
			std::vector<graphics::SpritedRenderable*> m_StandRenderables;
			std::vector<graphics::SpritedRenderable*> m_WalkRenderables;

		private:
			std::vector<int> actions; //this is to create a list of actions to do.
			//map::MapLayer* m_MapLayer;
			float m_MovementTimer;
			maths::mat4 m_TransformationMatrix;
		protected:
			maths::vec2 m_Force;
		public:
			Creature(maths::Rectangle bounds, map::MapLayer* m_MapLayer);
			virtual ~Creature();

			virtual void update() override;
			void draw(graphics::Renderer2D* renderable) const override;
			void move_to(const maths::vec2& pos);
		private:
			void analyze_controls();
			void change_stance(StanceState next_state);
			void addGravity(float& y_force) { y_force -= FORCE_OF_GRAVITY; }
			void multiplyByFriction(maths::vec2& force)
			{
				if (m_Foothold != NO_FOOTHOLD)
					force *= FRICTION;
				else
					force *= FRICTION_AIR;
			}

			//maths::vec3 getPosition() override { return m_TransformationMatrix.; }
		};
	}
}