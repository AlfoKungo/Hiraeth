#pragma once

#include "graphics/spritedrenderable.h"
#include "keyboard/keyboard.h"
#include "map/maplayer.h"
#include "graphics/texturemanager.h"
#include "physics/collisionable.h"
#include "stats.h"

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

#define CHARACTER_SPEED 25.0f
#define MONSTER_SPEED 15.0f
#define AIR_SPEED_MODIFIER calculateAirModifier()
#define CHARACTER_JUMP 13.0f
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
			Stats* m_Stats;

		private:
			float m_Speed, m_Jump; //link to stats_struct
			float m_HitTimer;
			std::vector<int> actions; //this is to create a list of actions to do.
			float m_MovementTimer;
			maths::mat4 m_TransformationMatrix;
		public:
			bool is_hit = false;
		public:
			Creature(maths::Rectangle bounds, map::MapLayer* m_MapLayer, float speed, float jump,
				Stats* stats);
			virtual ~Creature();

			virtual void update() override;
			void draw(graphics::Renderer2D* renderable) const override;
			void getHit(Direction dir, Damage damage);
			inline void move(const maths::vec2& step) override
			{
				m_Bounds.position += step;
				m_TransformationMatrix *= maths::mat4::Translate(m_Force);
			}
			Damage getDamage() const { return m_Stats->getDamage(); }
			Stats* getStats() const { return m_Stats; }
		private:
			void analyze_controls();
			void change_stance(StanceState next_state);
			void addGravity(maths::vec2& force) const { force.y -= FORCE_OF_GRAVITY; }
			float calculateForce(float force) const
			{
				if (m_Foothold != NO_FOOTHOLD)
					return force;
				return force * AIR_SPEED_MODIFIER;
			}
			maths::vec2 calculateForce(const maths::vec2& force) const
			{
				if (m_Foothold != NO_FOOTHOLD)
					return force;
				return maths::vec2(force.x * AIR_SPEED_MODIFIER, force.y);
			}
			float calculateAirModifier() const
			{
				return (m_Speed / (1 - FRICTION.x) * FRICTION.x)/m_Speed * (1-FRICTION_AIR.x);
			}
			void multiplyByFriction(maths::vec2& force) const
			{
				if (m_Foothold != NO_FOOTHOLD)
					force *= FRICTION;
				else
					force *= FRICTION_AIR;
			}
		};
	}
}