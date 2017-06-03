#pragma once

#include "utils/timer.h"
#include "graphics/sprited_renderable.h"
#include "graphics/texture.h"
#include "game/character.h"
#include "keyboard/keyboard.h"
#include "keyboard/keyboard_event.h"
#include "graphics/texture_manager.h"


namespace hiraeth {
	namespace map {
		class Map;
		class Portal : public graphics::SpritedRenderable<8>
		{

			//#define PORTAL_ANIMATION_DELAY 12.0f
#define PORTAL_ANIMATION_DELAY 0.12f
#define PORTAL_FRAME_WIDTH     110.0f

		public:
			struct Serializer
			{
				maths::vec3 position;
				int next_map;
				template<class Archive>
				void serialize(Archive & ar)
				{
					ar(position, next_map);
				}
			};
		private:
			int m_NextMap{};

		public:
			Portal(maths::vec3 position, int next_map, input::Keyboard* kb, Map* map, game::Character* character);
			Portal(Serializer s, input::Keyboard* kb, Map* map, game::Character* character)
				: Portal(s.position, s.next_map, kb, map, character) {}
			Portal(maths::vec3 position, int next_map)
				: Portal(position, next_map, nullptr, nullptr, nullptr) {}
			~Portal();

			int getNextMap() const { return m_NextMap; }

			template<class Archive>
			void serialize(Archive & ar)
			{
				ar(m_Bounds.position, m_NextMap);
			}
			template <class Archive>
			static void load_and_construct(Archive & ar, cereal::construct<Portal> & construct)
			{
				maths::vec2 pos;
				int next_map;
				ar(pos, next_map);
				construct(pos, next_map); // calls MyType( x )
			}
			//Portal() : SpritedRenderable(maths::vec2(0), 8, PORTAL_ANIMATION_DELAY, graphics::TextureManager::Load("portal_adv.png"))
			//{}
			friend class cereal::access;
		private:
		};
	}
}