
#include "network/client_handler.h"
#include "game/character.h"
#include "utils/static_timer.h"
#include "src/graphics/window.h"
#include "keyboard/keyboard.h"
#include "view/camera.h"
#include "src/graphics/shader.h"
#include "src/map/map.h"
#include "UI/ui_manager.h"
#include "game/monsters/monster_manager.h"
#include "item/item_manager.h"
#include "NPC/npc_manager.h"

#include <ft2build.h>
#include FT_FREETYPE_H


//#include <ga.h>
//#include <gau.h>

#include <cstdio>
#include "skills/skill_manager.h"
#include "game/net_chars/net_char_manager.h"
#include "editor/editor.h"
#include "basic/network_handler.h"
#include "basic/char_handler.h"

//#define EDITOR

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
	
#ifdef EDITOR
	editor::Editor editor;
	//editor.MobEditor();
	editor.MapEditor();
#endif
	
	input::Keyboard keyboard;
	Window window("Hiraeth", 1600, 900, &keyboard);

	//mat4 ortho = mat4::Orthographic(0.0f, 16.0f, 0.0f, 9.0f, -1.0f, 1.0f);

	StaticTimer::init();

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	float timer = 0;
	Map map(0, &window);

	ui::UiManager uiManager(&keyboard);

	item::ItemManager itemManager{ map.getMapLayer()->getFootHolds(), uiManager.getUiInventory(), uiManager.getUiEquip() };
	skills::SkillManager skillManager{ uiManager.getUiSkills(), uiManager.getMainUi()->getCharacterStats() };


	game::MonsterManager monsterManager(map.getMapLayer());
	game::NetCharManager netCharManager{ map.getMapLayer(), &keyboard, &skillManager, monsterManager.getMonsters() };

	network::ClientHandler clientHandler{ &uiManager, &netCharManager, &monsterManager, 
		&itemManager, &skillManager, map.getMapLayer() }; // itemManager, 
	NetworkManager::setHandler(&clientHandler);

	uiManager.getMainUi()->setCharacterStats(clientHandler.getPlayerData().player_stats,
		clientHandler.getPlayerData().stats_alloc);
	//uiManager.getUiSkills()->setClientHandler(&clientHandler);
	itemManager.setInvEquip(clientHandler.getPlayerData().inv_equip);
	itemManager.setInvUse(clientHandler.getPlayerData().inv_use);
	itemManager.setEquipsChar(clientHandler.getPlayerData().equips_char);
	std::vector<unsigned int> aval{ 0,1,2 };
	uiManager.getUiQuests()->setQuestsTab(aval, SRL::QuestTab::Available);
	for (auto [active_quest_id, quest_stage] : clientHandler.getPlayerData().quests_in_progress)
		uiManager.getUiQuests()->setQuestAsActive(active_quest_id);
	//uiManager.getUiQuests()->setQuestsInProgress(clientHandler.getPlayerData().quests_in_progress, SRL::QuestTab::InProgress);
	uiManager.getUiQuests()->setQuestsTab(clientHandler.getPlayerData().quests_done, SRL::QuestTab::Done);
	skillManager.setJobAndLoadSkills(clientHandler.getPlayerData().player_stats.job, clientHandler.getPlayerData().skills_alloc);

	graphics::Layer<game::Character> m_CrLayer(new Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag"), true);
	game::Character Char(maths::vec2(0, 0), &keyboard, map.getMapLayer(),
		uiManager.getUiEquip(), &uiManager, &itemManager,
		&skillManager, uiManager.getMainUi()->getCharacterStats(), monsterManager.getMonsters());
		//&clientHandler);
	CharManager::setChar(&Char);
	m_CrLayer.add_ref(&Char);
	view::Camera::init(&Char);

	game::NpcManager npcManager(map.getMapLayer(), &keyboard, &Char, uiManager.getUiQuests());


	unsigned int frames = 0;
	while (!window.closed())
	{
		//update
		window.clear();
		double x, y;
		window.getKeyboard()->getMousePosition(x, y);
		auto mouse_pos = window.getKeyboard()->getMousePosition();
		std::string s = "my name is : " + std::to_string(Char.getBounds().x) + ", "+ 
			std::to_string(Char.getBounds().y) + ", " + std::to_string(Char.getBounds().width) + ", "  + 
			std::to_string(Char.getBounds().height) + "; " + std::to_string(mouse_pos.x) + "," + std::to_string(mouse_pos.y);
		window.setTitle(s.c_str());
		Camera::update();
		map.update();
		monsterManager.update();
		skillManager.update();
		npcManager.update();
		m_CrLayer.update();
		itemManager.update();
		uiManager.update();
		clientHandler.Update(network::PlayerStateUpdateMsg{ Char.getPosition(), Char.getForce() , Char.getDirection()});
		netCharManager.update();
	

		//draw
		map.draw();
		monsterManager.draw();
		skillManager.draw();
		netCharManager.draw();
		npcManager.draw();
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