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

		protected:
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
			std::vector<graphics::SpritedRenderable*> m_StandRenderables;
			std::vector<graphics::SpritedRenderable*> m_WalkRenderables;

		private:
			std::vector<int> actions; //this is to create a list of actions to do.
			//map::MapLayer* m_MapLayer;
			float m_MovementTimer;
			maths::vec2 m_Force;
			maths::mat4 m_TransformationMatrix;
		public:
			Creature(maths::Rectangle bounds, map::MapLayer* m_MapLayer);
			virtual ~Creature();

			void update() override;
			void draw(graphics::Renderer2D* renderable) const override;
			void move_to(const maths::vec2& pos);
		private:
			void analyze_controls();
			void change_stance(StanceState next_state);
			void addGravity(float& y_force) { y_force -= FORCE_OF_GRAVITY; }
			void multiplyByFriction(maths::vec2& force) { force *= FRICTION; }

			//maths::vec3 getPosition() override { return m_TransformationMatrix.; }
		};
	}
}