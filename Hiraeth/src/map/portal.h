#pragma once

#include "utils/timer.h"
#include "graphics/spritedrenderable.h"
#include "graphics/texture.h"
#include "game/character.h"
#include "keyboard/keyboard.h"
#include "keyboard/keyboard_event.h"


namespace hiraeth {
	namespace map {
		class Map;
		class Portal : public graphics::SpritedRenderable, public input::KeyboardEvent
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
			enum State {
				Default,
				Change_Map
			} m_State;

			int m_NextMap;
			input::Keyboard* m_Kb;
			Map* m_Map;
			game::Character* m_Char;

		public:
			Portal(maths::vec3 position, int next_map, input::Keyboard* kb, graphics::Texture* ptex, Map* map, game::Character* character);
			Portal(Serializer s, input::Keyboard* kb, graphics::Texture* ptex, Map* map, game::Character* character)
				: Portal(s.position, s.next_map, kb, ptex, map, character) {}
			~Portal();

			void update() override;

			void ButtonClicked(input::Controls control) override;
			void ButtonReleased(input::Controls control) override {}
		private:

		};
	}
}