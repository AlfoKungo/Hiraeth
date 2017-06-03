#pragma once

#include "graphics/sprited_renderable.h"
#include "keyboard/keyboard.h"
#include "map/maplayer.h"
#include "graphics/texture_manager.h"
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
			bool attack = false;
		};

		enum StanceState {
			NoStance,
			Stand,
			Walk,
			Attack,
			Jump,
		};
		enum Direction {
			Right = 1,
			Left = -1,
		};

		class Creature : public graphics::Renderable, public physics::Collisionable
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
			std::map<StanceState, std::vector<std::unique_ptr<Renderable>>> m_StatesRenderables;
			std::vector<std::unique_ptr<Renderable>>* m_CurrentRenderables;
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
			void draw(graphics::Renderer* renderable) const override;
			void getHit(Direction dir, Damage damage);
			inline void move(const maths::vec2& step) override
			{
				m_Bounds.position += step;
				m_TransformationMatrix *= maths::mat4::Translate(m_Force);
			}
			Damage getDamage() const { return m_Stats->getDamage(); }
			Stats* getStats() const { return m_Stats; }
			virtual void attack() = 0;
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