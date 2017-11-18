#include <time.h>
#include <cereal/archives/binary.hpp>
#include <fstream>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>

#include "utils/static_timer.h"
#include "src/maths/maths.h"

#include "src/graphics/window.h"
#include "keyboard/keyboard.h"
#include "view/camera.h"

#include "src/graphics/shader.h"
#include "src/map/map.h"
#include "graphics/texture_manager.h"
#include "graphics/label.h"
#include "UI/ui_manager.h"
#include "game/monster_manager.h"
#include "item/item_manager.h"

#include <ft2build.h>
#include FT_FREETYPE_H

//#include <ga.h>
//#include <gau.h>

#include <stdio.h>

#if 0
static void setFlagAndDestroyOnFinish(ga_Handle* in_handle, void* in_context)
{
    gc_int32* flag = (gc_int32*)(in_context);
    *flag = 1;
    ga_handle_destroy(in_handle);
}
int main()
{
    gau_Manager* mgr;
    ga_Mixer* mixer;
    ga_Sound* sound;
    ga_Handle* handle;
    gau_SampleSourceLoop* loopSrc = 0;
    gau_SampleSourceLoop** pLoopSrc = &loopSrc;
    gc_int32 loop = 0;
    gc_int32 quit = 0;

    /* Initialize library + manager */
    gc_initialize(0);
    mgr = gau_manager_create();
    mixer = gau_manager_mixer(mgr);

    /* Create and play shared sound */
    if (!loop)
        pLoopSrc = 0;
    sound = gau_load_sound_file("All_Night.wav", "wav");
    handle = gau_create_handle_sound(mixer, sound, &setFlagAndDestroyOnFinish, &quit, pLoopSrc);
    ga_handle_play(handle);

    /* Bounded mix/queue/dispatch loop */
    while (!quit)
    {
        gau_manager_update(mgr);
        printf("%d / %d\n", ga_handle_tell(handle, GA_TELL_PARAM_CURRENT), ga_handle_tell(handle, GA_TELL_PARAM_TOTAL));
        gc_thread_sleep(1);
    }

    /* Clean up sound */
    ga_sound_release(sound);

    /* Clean up library + manager */
    gau_manager_destroy(mgr);
    gc_shutdown();

    return 0;
}
#else
int main()
{

	using namespace hiraeth;
	using namespace maths;
	using namespace graphics;
	using namespace map;
	using namespace view;

	input::Keyboard keyboard;
	Window window("Hiraeth", 1600, 900, &keyboard);

	//mat4 ortho = mat4::Orthographic(0.0f, 16.0f, 0.0f, 9.0f, -1.0f, 1.0f);

	StaticTimer::init();

	float timer = 0;
	Map map(0, &window);

	ui::UiManager uiManager(&keyboard);

	item::ItemManager itemManager{ map.getMapLayer()->getFootHolds(), uiManager.getUiInventory() };

	graphics::Layer<game::Character> m_CrLayer(new Shader("src/shaders/basic.vert", "src/shaders/basic.frag"), true);
	game::Character m_Char(maths::vec2(0, 0), &keyboard, map.getMapLayer(), &itemManager, uiManager.getMainUi()->getCharacterStats());
	m_CrLayer.add_ref(&m_Char);
	view::Camera::init(&m_Char);

	game::MonsterManager monsterManager(map.getMapLayer(), &m_Char, &itemManager);

	unsigned int frames = 0;
	while (!window.closed())
	{
		//update
		window.clear();
		double x, y;
		window.getKeyboard()->getMousePosition(x, y);
		std::string s = "my name is : " + std::to_string(x) + ", " + std::to_string(y);
		window.setTitle(s.c_str());
		Camera::update();
		map.update();
		monsterManager.update();
		m_CrLayer.update();
		itemManager.update();
		uiManager.update();


		//draw
		map.draw();
		monsterManager.draw();
		m_CrLayer.render();
		itemManager.draw();
		uiManager.draw();

		window.update();
		++frames;
		if (StaticTimer::timer.elapsed() - timer > 1.0f)
		{
			timer += 1.0f;
			printf("%d fps, %f frame time\n", frames, 1.0 / frames);
			frames = 0;
		}

	}
	return 0;
}
#endif