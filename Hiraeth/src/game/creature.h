#pragma once

#include "graphics/sprited_renderable.h"
#include "graphics/sprite.h"
#include "keyboard/keyboard.h"
#include "map/maplayer.h"
#include "base_creature.h"
#include "graphics/texture_manager.h"
#include "physics/collisionable.h"
#include "stats.h"
#include "basic/a_timer.h"

namespace hiraeth
{
	namespace game
	{

		const float PREHIT_DELAY = 0.35f, POSTHIT_DELAY = 0.25f, POSTHIT_MOVEABLE_DELAY = 0.2f;
		enum AttackState
		{
			PreHit,
			PostHit,
			PostHitMovable,
		};

		class Creature : public BaseCreature
		{

		public:
			bool is_hit = false;
			bool has_died = false;
		protected:
			AttackState m_AttackState;
			bool AttackStatus = false;
			ATimer m_AttackTimer;
			bool m_IsImmuneAfterHit;
			Stats* m_Stats;

			maths::vec2 m_HitBox;
			graphics::Sprite m_HitSprite;

		private:
			ATimer m_HitTimer;
			std::vector<int> actions; //this is to create a list of actions to do.
		public:
			Creature(maths::Rectangle bounds, map::MapLayer* map_layer,
			         Stats* stats, bool is_immune_after_hit);

			Creature(maths::Rectangle bounds, map::MapLayer* map_layer,
				float speed, float jump,
				Stats* stats, bool is_immune_after_hit);

			void update() override;
			virtual void getHit(Direction dir, Damage damage);

			Damage getDamage() const;
			//virtual Damage getDamage() const = 0;
			void draw(graphics::Renderer* renderer) const override
			{
				BaseCreature::draw(renderer);
				renderer->push(m_TransformationMatrix);
				renderer->submit(&m_HitSprite);
				renderer->pop();
			}
			maths::Rectangle getHitBox() const
			{
				return maths::Rectangle{ m_Bounds.position, m_HitBox };
			}
			virtual void carryOutAttack() = 0;
			void change_stance_to_attack();
			virtual void attackMonster(Creature* attacked, Damage d)
			{}
		private:
			//virtual void cause_damage(Damage damage) = 0;
			virtual void cause_damage(Damage damage)  { m_Stats->causeDamage(damage); }
			void apply_hit_state();
			void analyze_controls() override;
		};
	}
}
