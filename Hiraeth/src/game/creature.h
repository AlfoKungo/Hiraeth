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
#include "srl/equip_item_data.h"
#include "graphics/text_box.h"

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
		public:

			std::map<SRL::EquipItemType,  graphics::Sprite> m_Equips;

		private:
			ATimer m_HitTimer;
			std::vector<int> actions; //this is to create a list of actions to do.
			graphics::TextBox<graphics::BoxTexturedFull> m_SayBox;
			ATimer m_SayTimer;
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
				if (m_Direction == Left)
					renderer->push(m_TransformationMatrix * maths::mat4::Translate(-maths::vec3(m_Org)));
				else
					renderer->push(m_TransformationMatrix * maths::mat4::Translate(maths::vec3(m_Org)) *
						maths::mat4::Scale(maths::vec3(-1, 1, 1)) * maths::mat4::Translate(-maths::vec3(2 * m_Org)));
				for (auto [f, s] : m_Equips)
					s.draw(renderer);
				renderer->pop();
				renderer->push(m_TransformationMatrix);
			if (!m_SayTimer.hasExpired())
				m_SayBox.draw(renderer);
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
			void writeSayBubble(std::string say_text)
			{
				m_SayTimer.reSet(5.0f);
				m_SayBox.setText(say_text);
			}
		private:
			//virtual void cause_damage(Damage damage) = 0;
			virtual void cause_damage(Damage damage)  { m_Stats->causeDamage(damage); }
			void apply_hit_state();
			void analyze_controls() override;
		};
	}
}
