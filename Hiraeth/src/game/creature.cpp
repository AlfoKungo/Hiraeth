#include "creature.h"

namespace hiraeth
{
	namespace game
	{
		Creature::Creature(maths::Rectangle bounds, map::MapLayer* map_layer,
			float speed, float jump, Stats* stats,
			bool is_immune_after_hit)
			: BaseCreature(bounds, map_layer,
				//std::make_unique<float>(stats->Speed), std::make_unique<float>(stats->Jump), 
				speed, jump, 
				NO_FOOTHOLD),
			m_AttackState(),
			m_IsImmuneAfterHit(is_immune_after_hit),
			m_Stats(stats),
			m_HitSprite(maths::vec2(0, 0), 0, 0, 0xe066ccff)
		{
		}

		Creature::Creature(maths::Rectangle bounds, map::MapLayer* map_layer,
			Stats* stats, bool is_immune_after_hit)
			: Creature(bounds, map_layer, stats->getSpeed(), stats->getJump(), stats, is_immune_after_hit)
		{
		}

		void Creature::update()
		{
			if (m_StanceState == Die)
			{
				if (m_StatesRenderables[Die][0]->hasSpriteFinished())
					has_died = true;
				BaseCreature::update();
				return;
			}
			apply_hit_state();
			//m_Controls.attack = true;
			//m_Controls.left = true;

			BaseCreature::update();
			if (m_StanceState == Attack)
				switch (m_AttackState)
				{
				case PreHit:
					if (m_Foothold != NO_FOOTHOLD)
						m_Force.x = 0;
					if (m_AttackTimer.hasExpired())
					{
						//carryOutAttack();
						m_AttackState = PostHit;
						m_AttackTimer.reSet(POSTHIT_DELAY);
					}
					break;
				case PostHit:
					if (m_Foothold != NO_FOOTHOLD)
						m_Force.x = 0;
					if (m_AttackTimer.hasExpired())
					{
						m_AttackState = PostHitMovable;
						m_AttackTimer.reSet(POSTHIT_MOVEABLE_DELAY);
					}
					break;
				case PostHitMovable:
					if (m_AttackTimer.hasExpired() || (m_Force.x < -0.1f || m_Force.x > 0.1f))
					{
						m_StanceState = NoStance;
						analyze_controls();
						analyze_stance();
					}
					break;
				}
			analyze_stance();
		}

		void Creature::change_stance_to_attack()
		{
				change_stance(Attack);
				m_AttackState = PreHit;
				m_AttackTimer.reSet(PREHIT_DELAY);
				AttackStatus = true;
		}

		void Creature::apply_hit_state()
		{
			if (is_hit && m_IsImmuneAfterHit)
			{
				if (m_HitTimer.hasExpired())
				{
					is_hit = false;
					m_Color = 0xffffffff;
				}
				else
					m_Color = 0x00ffffff | ((unsigned char((m_HitTimer.timeRemain()) / 1.5f * 128 * 2) % 128 + 86) << 24
					);
			}
		}

		void Creature::analyze_controls()
		{
			if ((m_Controls.attack && Attack != m_StanceState) && (!(Attack == m_StanceState && PostHitMovable == m_AttackState)))
			{
				//change_stance(Attack);
				//m_AttackState = PreHit;
				//m_AttackTimer.reSet(0.35f);
				//AttackStatus = true;
				carryOutAttack();
				change_stance_to_attack();
				return;
			}
			if (Attack == m_StanceState && (PreHit == m_AttackState || PostHit == m_AttackState))
				return;
			BaseCreature::analyze_controls();
		}

		void Creature::getHit(Direction dir, Damage damage)
		{
			if (!is_hit)
			{
				cause_damage(damage);
				m_Force = calculate_force(maths::vec2(dir * 3, 7));
				m_Foothold = NO_FOOTHOLD;
				if (m_IsImmuneAfterHit)
				{
					is_hit = true;
					m_HitTimer.reSet(1.5f);
					m_Color &= 0x80ffffff;
				}
			}
		}

		Damage Creature::getDamage() const
		{
			return m_Stats->getDamage();
		}
	}
}
