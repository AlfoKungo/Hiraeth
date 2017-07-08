#pragma once
#include "basic/drawable.h"
#include "basic/updatable.h"
#include "graphics/renderable.h"
#include <vector>
#include "graphics/layers/layer.h"
#include "graphics/layers/group.h"
#include "graphics/sprite.h"
#include "graphics/texture_manager.h"
#include "game/character_stats.h"
#include "graphics/label.h"
#include <string>


namespace hiraeth {
	namespace ui {
		class MainUi : public Updatable
		{
		private:
			game::CharacterStats m_CharacterStats;
			graphics::Sprite *m_Hp, *m_Mp, *m_Exp;
			game::StatsStruct* m_StatsStruct;
			graphics::Layer<graphics::Renderable> m_Layer;
			graphics::Group *m_GraphicGroup, *m_LabelGroup;
		public:
			explicit MainUi();
			void update() override;
			void draw() const;
			void StatsUpdated();
			void StatsUpdatedT(int X);
			game::CharacterStats * getCharacterStats() { return &m_CharacterStats; }
		private:
			void fill_stats_group();
			std::string create_stats_string(unsigned int value, unsigned int maxValue) const ;
		};
	}
}
