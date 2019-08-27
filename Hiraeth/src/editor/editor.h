#pragma once
#include "keyboard/keyboard.h"
#include "graphics/window.h"
#include "utils/static_timer.h"
#include "view/camera.h"
#include "game/character_graphics.h"
#include "mob_animation.h"
#include "mob_controller.h"

//#include "src/graphics/shader.h"

namespace hiraeth {
	namespace editor {

		class Editor
		{
		public:
			void Main()
			{
				input::Keyboard keyboard;
				graphics::Window window("Hiraeth", 1600, 900, &keyboard);

				//mat4 ortho = mat4::Orthographic(0.0f, 16.0f, 0.0f, 9.0f, -1.0f, 1.0f);

				StaticTimer::init();

				float timer = 0;

				graphics::Layer<graphics::Renderable> MainLayer(new graphics::Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag"), true);
			MainLayer.add(new graphics::Sprite(maths::vec2(-900, -450), new graphics::Texture("bg.png", 0)));
				//game::Character m_Char(maths::vec2(0, 0), &keyboard, map.getMapLayer(), &itemManager,
				//	&skillManager, uiManager.getMainUi()->getCharacterStats(), monsterManager.getMonsters(),
				//	&clientHandler);
				unsigned int mob_id = 0;
				
				MobAnimation ma{ 1 };
				MobController mb{ &ma , &keyboard};
			//	graphics::SpritedRenderable anima{maths::vec2{ 0 },
			//		SRL::AnimationData{ {{{0, 0, 45, 35}, {11, 29}, 0.4f}, {{45, 0, 45, 35}, {11, 29}, 0.4f}, {{90, 0, 45, 35}, {11, 29}, 0.4f}}, true },
			//		graphics::TextureManager::Load("char_head.png")};
			//MainLayer.add(new character::CharacterBody(	
			//	SRL::AnimationData{ {{{0, 0, 45, 33}, {19, 0}, 0.35f}, {{45, 0, 45, 33}, {19, 0}, 0.45f }}, false },
			//	graphics::TextureManager::Load("char_body_attack.png")));
			//MainLayer.add(new character::CharacterArm(
			//	SRL::AnimationData{ {{{0, 0, 30, 18}, {49, 10}, 0.35f}, {{30, 0, 30, 18}, {0, 11}, 0.45f}}, false },
			//	graphics::TextureManager::Load("char_hand_attack.png"), std::vector<maths::vec2>{ maths::vec2(47, 10), maths::vec2(-8, 19)}));
			//MainLayer.add(new graphics::SpritedRenderable(maths::vec2{ 0 },
			//	SRL::AnimationData{ {{{0, 0, 45, 35}, {25, 27}, 0.35f}, {{45, 0, 45, 35}, {8, 26}, 0.45f}}, false },
			//	graphics::TextureManager::Load("char_head_attack.png")));
				MainLayer.add_ref(&ma);
				//graphics::Renderable focus_rend{ maths::vec3{0,0,0}, maths::vec2{100,100}, 0x55555555 };
				view::Camera::init(&ma);

				//game::NpcManager npcManager(map.getMapLayer(), &keyboard, &m_Char, uiManager.getUiQuests());

				unsigned int frames = 0;
				while (!window.closed())
				{
					//update
					window.clear();
					double x, y;
					window.getKeyboard()->getMousePosition(x, y);
					//std::string s = "my name is : " + std::to_string(m_Char.getBounds().x) + ", " + std::to_string(m_Char.getBounds().y) + ", " + std::to_string(m_Char.getBounds().width) + ", " + std::to_string(m_Char.getBounds().height);
					//window.setTitle(s.c_str());
					view::Camera::update();
					MainLayer.update();


					//draw
					MainLayer.render();

					window.update();
					++frames;
					if (StaticTimer::timer.elapsed() - timer > 1.0f)
					{
						timer += 1.0f;
						printf("%d fps, %f frame time\n", frames, 1.0 / frames);
						frames = 0;
					}

				}
			}
		};
	}
}
