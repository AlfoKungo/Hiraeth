#pragma once

#include "utils/timer.h"
#include "graphics/sprited_renderable.h"
#include "graphics/texture.h"
#include "keyboard/keyboard.h"
#include "keyboard/keyboard_event.h"
#include "graphics/texture_manager.h"
#include "srl/map_data.h"
#include <cereal/access.hpp>


namespace hiraeth {
	namespace map {
		class Portal : public graphics::SpritedRenderable
		{

			//#define PORTAL_ANIMATION_DELAY 12.0f
#define PORTAL_ANIMATION_DELAY 0.12f
#define PORTAL_FRAME_WIDTH     110.0f

		private:
			int m_NextMap;

		public:
			Portal(maths::vec3 position, int next_map);
			Portal(SRL::PortalData pl_data)
				: Portal(pl_data.position, pl_data.next_map) {}
			~Portal();

			int getNextMap() const { return m_NextMap; }
		private:
		};
	}
}
