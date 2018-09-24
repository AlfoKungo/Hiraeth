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
			NoStance = 0,
			Stand = 1,
			Walk = 2,
			Attack = 3,
			Jump = 4,
			Skill = 5,
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
			StanceState m_StanceState;
			Direction m_Direction;
			CreatureControls m_Controls;
			std::map<StanceState, std::vector<std::unique_ptr<Renderable>>> m_StatesRenderables;
			std::vector<std::unique_ptr<Renderable>>* m_CurrentRenderables;
			maths::mat4 m_TransformationMatrix;

			float m_Speed, m_Jump; //link to stats_struct

		private:
			std::vector<int> actions; //this is to create a list of actions to do.
		public:
			bool is_hit = false;

			BaseCreature(maths::Rectangle bounds, map::MapLayer* map_layer,
				float speed, float jump,
				unsigned int foothold);

			void update() override;
			void draw(graphics::Renderer* renderer) const override;
			void move(const maths::vec2& step) override
			{
				m_Bounds.position += step;
				m_TransformationMatrix *= maths::mat4::Translate(step);
			}
		protected:
			virtual void analyze_controls();
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
				return ((m_Speed) / (1 - FRICTION.x) * FRICTION.x) / (m_Speed) * (1 - FRICTION_AIR.x);
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
