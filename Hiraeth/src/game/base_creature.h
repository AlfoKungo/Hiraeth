#pragma once

#include "graphics/sprited_renderable.h"
#include "keyboard/keyboard.h"
#include "map/maplayer.h"
#include "graphics/texture_manager.h"
#include "physics/collisionable.h"
#include "stats.h"
#include "basic/a_timer.h"

namespace hiraeth {
	namespace game {

		enum StanceState {
			NoStance,
			Stand,
			Walk,
			Attack,
			Jump,
		};
		struct CreatureControls
		{
			bool left = false;
			bool right = false;
			bool up = false;
			bool down = false;
			bool jump = false;
			bool attack = false;
			bool pick_up = false;
		};
		//enum AttackState {
		//	PreHit,
		//	PostHit,
		//	PostHitMovable,
		//};
		enum Direction {
			Right = 1,
			Left = -1,
		};

		class BaseCreature : public graphics::Renderable, public physics::Collisionable
		{

#define AIR_SPEED_MODIFIER calculate_air_modifier()
#define FRICTION maths::vec2(0.2f, 1.0f)
#define FRICTION_AIR maths::vec2(0.99f, 1.0f)
#define CHARACTER_TIME_BETWEEN_ADDS (1.0f/60.0f)
#define FORCE_OF_GRAVITY 0.94f

		protected:
			//AttackState m_AttackState;
			ATimer m_AttackTimer;
			//bool m_IsImmuneAfterHit;
			StanceState m_StanceState;
			Direction m_Direction;
			CreatureControls m_Controls;
			std::map<StanceState, std::vector<std::unique_ptr<Renderable>>> m_StatesRenderables;
			std::vector<std::unique_ptr<Renderable>>* m_CurrentRenderables;
			//Stats * m_Stats;
			maths::mat4 m_TransformationMatrix;

		private:
			//float &m_Speed, &m_Jump; //link to stats_struct
			std::unique_ptr<float> m_Speed, m_Jump; //link to stats_struct
			ATimer m_HitTimer;
			std::vector<int> actions; //this is to create a list of actions to do.
		public:
			bool is_hit = false;

			BaseCreature(maths::Rectangle bounds, map::MapLayer* map_layer,
				std::unique_ptr<float> speed, std::unique_ptr<float> jump,
				unsigned int foothold);
			//virtual ~BaseCreature() = default;

			void update() override;
			void draw(graphics::Renderer* renderer) const override;
			void move(const maths::vec2& step) override
			{
				m_Bounds.position += step;
				//m_TransformationMatrix *= maths::mat4::Translate(m_Force);
				m_TransformationMatrix *= maths::mat4::Translate(step);
			}
			//virtual void attack() = 0;
		private:
			void analyze_controls();
			void analyze_stance();
			void change_stance(StanceState next_state);
			static void add_gravity(maths::vec2& force) { force.y -= FORCE_OF_GRAVITY; }
			float calculate_force(float force) const
			{
				if (m_Foothold != NO_FOOTHOLD)
					return force;
				return force * AIR_SPEED_MODIFIER;
			}
			maths::vec2 calculate_force(const maths::vec2& force) const
			{
				if (m_Foothold != NO_FOOTHOLD)
					return force;
				return {force.x * AIR_SPEED_MODIFIER, force.y};
			}
			float calculate_air_modifier() const
			{
				return ((*m_Speed) / (1 - FRICTION.x) * FRICTION.x) / (*m_Speed) * (1 - FRICTION_AIR.x);
			}
			void multiply_by_friction(maths::vec2& force) const
			{
				if (m_Foothold != NO_FOOTHOLD)
					force *= FRICTION;
				else
					force *= FRICTION_AIR;
			}
		};
	}
}
