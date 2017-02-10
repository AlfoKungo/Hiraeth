#pragma once

//#include "graphics\spritedrenderable.h"
//#include "graphics\window.h"
//#include "map\maplayer.h"
//#include "graphics\texturemanager.h"
#include "game\creature.h"

namespace hiraeth {
	namespace game {

		class Character : public Creature
		{

		private:
		public:
			Character(maths::vec3 pos, Timer* time, input::Keyboard* kb, 
				map::MapLayer* m_MapLayer);
			~Character();

			//void update() override;
		private:
			virtual CreatureControls set_update_controls() override;
		};
	}
}