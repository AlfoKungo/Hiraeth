#pragma once
#include <utility>
#include "graphics/sprite.h"
#include "keyboard/keyboard.h"
#include <cereal/archives/json.hpp>
#include "graphics/window.h"
#include "map/maplayer.h"
#include "graphics/layers/layer.h"
#include "srl/map_data.h"
#include "srl/tile_texture_data.h"
#include "srl/npc_data.h"
#include "graphics/label.h"

namespace hiraeth {
	namespace editor {
		class MapController : public input::MouseEvent, public input::KeyboardEvent
		{
			input::Keyboard* m_Keyboard;
			maths::vec2 m_Size;
			maths::vec2 m_Shift;
			graphics::Renderable* m_Cam;
			unsigned int m_TileIndex{};
			std::vector<SRL::TileUv> m_TilesUv;
			std::vector<SRL::TileData>* m_Tiles;
			unsigned int m_RandBegin{ 0 }, m_RandRange{ 6 };
			unsigned int m_MapIndex{ 0 };
			SRL::MapData* m_MapData;
			graphics::Layer<graphics::Renderable> m_FootholdsLayer;
			graphics::Layer<graphics::Renderable> m_PortalsLayer;
			graphics::Layer<graphics::Renderable> m_NpcLayer;
			graphics::Layer<graphics::Renderable> m_MobsLayer;
			graphics::Layer<graphics::Renderable> m_UiLayer;
			graphics::Label m_MainEditStateLabel, m_EditStateLabel, m_SelectionLabel;
			graphics::Sprite m_SelectionSquare;
			std::vector<unsigned int> m_GroupingList{ 6, 2, 2, 3, 4, 1, 1, 1, 1,2 ,2 };
			std::vector<unsigned int> m_SelectionVector{};
			unsigned int m_FhTilesStart{ 13 }, m_FhTilesLength{ 4 };
			unsigned int m_FhSTilesStart{ 8 }, m_FhSTilesLength{ 2 };
			unsigned int m_FhUTilesStart{ 17 };
			unsigned int m_FhDTilesStart{ 18 };
			//std::vector<unsigned int> m_FootholdTiles;
			//std::vector<SRL::TileData> m_CopyBuffer{};
			std::vector<unsigned int> m_CopyBuffer{};
			enum MainEditState
			{
				Tiles,
				Footholds,
				Portals,
				Npcs,
				Mobs,
			} m_MainEditState{ Tiles };
			enum EditState
			{
				Add,
				Move,
				Delete,
			} m_EditState{ Add };
			bool m_ShiftPressed{ false };
			int m_SelectedTile{ -1 };
			map::MapLayer* m_MapLayer;
			maths::vec2 m_LastPos{ 0 }, m_LastTileSize{ 0 };
			maths::vec2 m_NewTileSize{ 0 };
			bool m_MouseHold{ false };
			struct CamControl
			{
				bool left{ false }, right{ false };
			};
		public:
			MapController(std::vector<SRL::TileUv> tiles_data, map::MapLayer* map_layer,
				std::vector<SRL::TileData>* tiles, SRL::MapData* map_data,
				input::Keyboard* keyboard, maths::vec2 size, graphics::Renderable* cam)
				: m_Keyboard{ keyboard },
				m_Size(size),
				m_Cam(cam),
				m_TilesUv(std::move(tiles_data)),
				m_Tiles{ tiles },
				m_MapData(map_data),
				m_FootholdsLayer(new graphics::Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag"), true),
				m_PortalsLayer(new graphics::Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag"), true),
				m_NpcLayer(new graphics::Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag"), true),
				m_MobsLayer(new graphics::Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag"), true),
				m_UiLayer(new graphics::Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag")),
				m_MapLayer{ map_layer },
				m_MainEditStateLabel{ "arial", 15, "Main: Tiles", maths::vec2{-650, 420}, 0xffffffff },
				m_EditStateLabel{ "arial", 15, "Mode: Add", maths::vec2{-650, 400}, 0xffffffff },
				m_SelectionLabel{ "arial", 15, "Adding: {0, 6}", maths::vec2{-650, 380}, 0xffffffff },
				m_SelectionSquare{ maths::vec2{0,0}, 0, 0, 0xaaffffff }
			{
				loadFootholdsLayer();
				for (const auto& portal : m_MapData->Portals)
				{
					m_PortalsLayer.add(new graphics::Label("arial", 20, portal.next_map,
						portal.position + maths::vec2{ 30, 40 }, 0xff000000, graphics::Label::Alignment::CENTER));
					m_PortalsLayer.add(new graphics::Sprite{ portal.position,
						60.0f,	80.0f, 0xaaffaa45 });
				}
				for (const auto& summon : m_MapData->Summons)
				{
					m_MobsLayer.add(new graphics::Label("arial", 20, summon.monster_type,
						summon.position + maths::vec2{ 30, 40 }, 0xff000000, graphics::Label::Alignment::CENTER));
					m_MobsLayer.add(new graphics::Sprite{ summon.position,
						60.0f,	80.0f , 0xaa88d8fc });
				}
				for (const auto& npc : m_MapData->Npcs)
				{
					auto npc_data = SRL::deserial<SRL::NpcData>("npc", npc);
					maths::vec2 npc_pos{ npc_data.info.npc_x_value, 0 };
					m_NpcLayer.add(new graphics::Label("arial", 20, npc,
						npc_pos + maths::vec2{ 30, 40 }, 0xff000000, graphics::Label::Alignment::CENTER));
					m_NpcLayer.add(new graphics::Sprite{ npc_pos,
						60.0f,	80.0f , 0xaaaeff80 });
				}
				m_NewTileSize = m_TilesUv[0].UvSize;
				m_Shift = maths::vec2{ m_TilesUv[0].UvSize.x / 2.0f, -m_TilesUv[0].UvSize.y / 2.0f };
				m_UiLayer.add_ref(&m_MainEditStateLabel);
				m_UiLayer.add_ref(&m_EditStateLabel);
				m_UiLayer.add_ref(&m_SelectionLabel);
				m_UiLayer.add_ref(&m_SelectionSquare);
				m_Keyboard->registerToMouse(this);
				std::vector<input::KeyCode> keys_to_register
				{
					GLFW_KEY_LEFT_SHIFT,GLFW_KEY_RIGHT, GLFW_KEY_LEFT,
					GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_Z,
					GLFW_KEY_X, GLFW_KEY_C,	GLFW_KEY_Q,
					GLFW_KEY_F, GLFW_KEY_P, GLFW_KEY_DELETE,
					GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3,
					GLFW_KEY_4, GLFW_KEY_5,
				};
				for (const auto& key : keys_to_register)
					m_Keyboard->registerToKey(key, this);

				//m_Keyboard->registerToKey(GLFW_KEY_LEFT_SHIFT, this);
				//m_Keyboard->registerToKey(GLFW_KEY_RIGHT, this);
				//m_Keyboard->registerToKey(GLFW_KEY_LEFT, this);
				//m_Keyboard->registerToKey(GLFW_KEY_UP, this);
				//m_Keyboard->registerToKey(GLFW_KEY_DOWN, this);
				//m_Keyboard->registerToKey(GLFW_KEY_Z, this);
				//m_Keyboard->registerToKey(GLFW_KEY_X, this);
				//m_Keyboard->registerToKey(GLFW_KEY_C, this);
				//m_Keyboard->registerToKey(GLFW_KEY_Q, this);
				//m_Keyboard->registerToKey(GLFW_KEY_F, this);
				////m_Keyboard->registerToKey(GLFW_KEY_Y, this);
				//m_Keyboard->registerToKey(GLFW_KEY_P, this);
				//m_Keyboard->registerToKey(GLFW_KEY_DELETE, this);
			}

			void update()
			{
				m_FootholdsLayer.update();
				m_PortalsLayer.update();
				m_MobsLayer.update();
				m_NpcLayer.update();
				m_UiLayer.update();
				if (!m_ShiftPressed)
				{
					if (m_Keyboard->isKeyPressed(GLFW_KEY_D))
						m_Cam->setPosition(m_Cam->getPosition() + maths::vec2{ 10.0f, 0.0f });
					if (m_Keyboard->isKeyPressed(GLFW_KEY_A))
						m_Cam->setPosition(m_Cam->getPosition() - maths::vec2{ 10.0f, 0.0f });
					if (m_Keyboard->isKeyPressed(GLFW_KEY_W))
						m_Cam->setPosition(m_Cam->getPosition() + maths::vec2{ 0.0f, 10.0f });
					if (m_Keyboard->isKeyPressed(GLFW_KEY_S))
						m_Cam->setPosition(m_Cam->getPosition() - maths::vec2{ 0.0f, 10.0f });
				}
			}

			void draw()
			{
				m_FootholdsLayer.render();
				m_PortalsLayer.render();
				m_MobsLayer.render();
				m_NpcLayer.render();
				m_UiLayer.render();
			}

			void setModeLabel(EditState new_mode);
			void updateModeLabels();
			void setMainModeLabel(MainEditState new_mode);

			void ButtonClicked(input::Key control) override
			{
				if (control == GLFW_KEY_LEFT_SHIFT)
					m_ShiftPressed = true;
				else if (control == GLFW_KEY_1)
					setMainModeLabel(Tiles);
				else if (control == GLFW_KEY_2)
					setMainModeLabel(Footholds);
				else if (control == GLFW_KEY_3)
					setMainModeLabel(Portals);
				else if (control == GLFW_KEY_4)
					setMainModeLabel(Npcs);
				else if (control == GLFW_KEY_5)
					setMainModeLabel(Mobs);
				else if (control == GLFW_KEY_Z)
					setModeLabel(Add);
				else if (control == GLFW_KEY_X)
					setModeLabel(Move);
				else if (control == GLFW_KEY_C)
					setModeLabel(Delete);
				else if (control == GLFW_KEY_F)
				{

					m_MapData->FootHolds.erase(std::remove_if(m_MapData->FootHolds.begin(), m_MapData->FootHolds.end(),
						[](SRL::FootHoldData fh) {return fh.direction == maths::vec2{ 0, 1 }; }), m_MapData->FootHolds.end());
					for (const auto& tile : m_MapData->Tiles)
					{
						if (tile.type >= m_FhTilesStart && tile.type < m_FhTilesStart + m_FhTilesLength)
						{
							maths::vec2 p1{ tile.position.x - 5, tile.position.y + m_TilesUv[tile.type].UvSize.y - 5 };
							maths::vec2 p2{ tile.position.x + 5 + m_TilesUv[tile.type].UvSize.x, tile.position.y + m_TilesUv[tile.type].UvSize.y - 5 };
							m_MapData->FootHolds.push_back(SRL::FootHoldData{ p1, p2, {0,1} });
						}
						if (tile.type >= m_FhSTilesStart && tile.type < m_FhSTilesStart + m_FhSTilesLength)
						{
							maths::vec2 p1{ tile.position.x - 3, tile.position.y + m_TilesUv[tile.type].UvSize.y - 4 };
							maths::vec2 p2{ tile.position.x + 3 + m_TilesUv[tile.type].UvSize.x, tile.position.y + m_TilesUv[tile.type].UvSize.y - 4 };
							m_MapData->FootHolds.push_back(SRL::FootHoldData{ p1, p2, {0,1} });
						}
						if (tile.type == m_FhUTilesStart)
						{
							maths::vec2 p1{ tile.position.x - 3, tile.position.y + m_TilesUv[tile.type].UvSize.y - 63};
							maths::vec2 p2{ tile.position.x + 3 + m_TilesUv[tile.type].UvSize.x, tile.position.y+ m_TilesUv[tile.type].UvSize.y };
							m_MapData->FootHolds.push_back(SRL::FootHoldData{ p1, p2, {0,1} });
						}
						if (tile.type == m_FhDTilesStart)
						{
							maths::vec2 p1{ tile.position.x - 3, tile.position.y + m_TilesUv[tile.type].UvSize.y };
							maths::vec2 p2{ tile.position.x + 3 + m_TilesUv[tile.type].UvSize.x, tile.position.y + m_TilesUv[tile.type].UvSize.y - 63 };
							m_MapData->FootHolds.push_back(SRL::FootHoldData{ p1, p2, {0,1} });
						}
					}
					m_MapData->FootHolds.push_back(SRL::FootHoldData{ maths::vec2 {800, -150}, maths::vec2{1000, -150} , maths::vec2{ 0,1 } });
					m_MapData->FootHolds.push_back(SRL::FootHoldData{ maths::vec2 {800, -50}, maths::vec2{1000, -50} , maths::vec2{ 0,1 } });
				loadFootholdsLayer();
				}
				else if (control == GLFW_KEY_Q)
					if (m_Keyboard->isKeyPressed(GLFW_KEY_LEFT_CONTROL))
					{
						//m_MapData->Portals[0].position.y = -100;
						//m_MapData->Portals[1].position = maths::vec2{ 600 ,-100 };
						//m_MapData->FootHolds
						std::ofstream out("output/" + std::to_string(m_MapIndex) + ".json", std::ios::out | std::ios::binary);
						cereal::JSONOutputArchive ar(out);
						ar(*m_MapData);
					}
				if (!m_SelectionVector.empty())
				{
					auto multi = 1.0f;
					if (m_ShiftPressed)
						multi = 20.0f;
					if (control == GLFW_KEY_RIGHT)
						moveTiles(maths::vec2{ 1.0f, 0.0f } *multi);
					if (control == GLFW_KEY_LEFT)
						moveTiles(maths::vec2{ -1.0f, 0.0f }*multi);
					if (control == GLFW_KEY_UP)
						moveTiles(maths::vec2{ 0.0f, 1.0f }*multi);
					if (control == GLFW_KEY_DOWN)
						moveTiles(maths::vec2{ 0.0f, -1.0f }*multi);
				}
				//if (control == 9)
				//{
				//	for (const auto& id : m_SelectionVector)
				//		m_CopyBuffer.push_back(id);
				//}
				if (control == GLFW_KEY_P)
				{
					maths::vec2 shift = m_Keyboard->getMousePosition() - maths::vec2{ (graphics::WINDOW_WIDTH / 2), (graphics::WINDOW_HEIGHT / 2) }
					+(view::Camera::getCharacterPos() * maths::vec2{ 1.0f, -1.0f });
					shift = maths::vec2{ shift.x, -shift.y };
					const maths::vec2 base_shift{ (*m_Tiles)[m_SelectionVector[0]].position };
					std::vector<unsigned int> temp;
					for (const auto& id : m_SelectionVector)
					{
						addTile(SRL::TileData{ (*m_Tiles)[id].position + shift - base_shift,
							1.0f, (*m_Tiles)[id].type });
						temp.push_back(m_Tiles->size() - 1);
					}
					m_SelectionVector.erase(m_SelectionVector.begin(), m_SelectionVector.end());
					std::move(temp.begin(), temp.end(), std::back_inserter(m_SelectionVector));
					setSelectionLabel();
				}
				else if (control == GLFW_KEY_DELETE)
				{
					for (auto id = m_SelectionVector.rbegin(); id != m_SelectionVector.rend(); ++id)
					{
						//for (const auto& id : m_SelectionVector)
						deleteTile(*id);
					}
					m_SelectionVector.erase(m_SelectionVector.begin(), m_SelectionVector.end());
					setSelectionLabel();
				}
			}

			void ButtonReleased(input::Key control) override
			{
				if (control == GLFW_KEY_LEFT_SHIFT)
				{
					m_ShiftPressed = false;
					m_SelectedTile = -1;
				}
			}

			bool leftButtonClicked(float mx, float my) override
			{
				if (m_ShiftPressed)
				{
					m_SelectionSquare.setPosition({ mx - (graphics::WINDOW_WIDTH / 2), -my + (graphics::WINDOW_HEIGHT / 2) });
				}
				else
				{
					int temp;
					switch (m_EditState)
					{
					case Add:
						m_SelectedTile = findCurrentTile(mx, -my);
						m_MouseHold = true;
						setNewTile(maths::vec2{ mx, -my });
						break;
					case Move:
						//m_SelectionSquare.setPosition({ mx - (graphics::WINDOW_WIDTH / 2), -my + (graphics::WINDOW_HEIGHT / 2) });
						temp = findCurrentTile(mx, -my);
						if (temp != -1)
						{
							if (std::find(m_SelectionVector.begin(), m_SelectionVector.end(), temp) == m_SelectionVector.end())
							{
								m_SelectionVector.erase(m_SelectionVector.begin(), m_SelectionVector.end());
								m_SelectionVector.push_back(temp);
								m_MouseHold = true;
								setSelectionLabel();
							}
						}
						else
						{
							m_SelectionVector.erase(m_SelectionVector.begin(), m_SelectionVector.end());
							m_SelectionSquare.setPosition({ mx - (graphics::WINDOW_WIDTH / 2), -my + (graphics::WINDOW_HEIGHT / 2) });
							setSelectionLabel();
						}
						//m_MouseHold = true;
						//findCurrentTile(mx, -my);
						//setSelectionLabel();
						break;
					case Delete:
						deleteTile(mx, my);
						break;
					default:
						break;
					}
					//if (m_ShiftPressed)
					//{
					//	m_SelectedTile = findCurrentTile(mx, -my);
					//}
				}
				return true;
			}

			void setSelectionLabel();

			bool leftButtonReleased(float mx, float my) override
			{
				m_MouseHold = false;
				if (m_SelectionSquare.getSize() != maths::vec2{ 0 })
				{
					maths::vec2 p1{ m_SelectionSquare.getPosition() }, p2{ m_SelectionSquare.getSize() };
					if (p2.x < 0)
					{
						p1.x += p2.x;
						p2.x *= -1;
					}
					if (p2.y < 0)
					{
						p1.y += p2.y;
						p2.y *= -1;
					}
					maths::Rectangle selection_rec{ p1 + maths::vec2{view::Camera::getCharacterPos()}
						, p2 };
					m_SelectionVector.erase(m_SelectionVector.begin(), m_SelectionVector.end());
					unsigned int i = 0;
					for (const auto& tile : *m_Tiles)
					{
						if (selection_rec.Intersects(maths::Rectangle{ tile.position, m_TilesUv[tile.type].UvSize }))
							m_SelectionVector.push_back(i);
						i++;
					}
					m_SelectionSquare.setSize({ 0 });
					setSelectionLabel();
				}
				//m_SelectTile = false;
					//m_SelectedTile = -1;
				return true;
			}
			bool rightButtonClicked(float mx, float my) override
			{
				switch (m_EditState)
				{
				case Add:
					setUvAdd(mx, my);
					break;
				case Move:
					m_SelectionVector.erase(m_SelectionVector.begin(), m_SelectionVector.end());
					setSelectionLabel();
					break;
				default:
					break;
				}
				return true;
			}

			void setUvAdd(float mx, float my);

			bool mouseMove(float pmx, float pmy, float mx, float my)  override
			{
				//if (m_ShiftPressed)
				//if (m_ShiftPressed && m_Keyboard->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
				if (m_EditState == Move && m_Keyboard->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && m_SelectionVector.empty())
				{
					maths::vec2 rel_pos = { mx - (graphics::WINDOW_WIDTH / 2), -my + (graphics::WINDOW_HEIGHT / 2) };
					m_SelectionSquare.setSize(rel_pos - m_SelectionSquare.getPosition());

				}
				else
				{
					switch (m_EditState)
					{
					case Add:
						if (m_MouseHold)
						{
							//maths::vec2 new_pos = maths::vec2{ mx - 900, -my + 450 } +m_Cam->getPosition() + m_Shift;
							maths::vec2 new_pos = maths::vec2{ mx, -my };
							if (new_pos.x < m_LastPos.x - m_NewTileSize.x)
								setNewTile(maths::vec2{ m_LastPos.x - m_NewTileSize.x, m_LastPos.y });
							if (new_pos.x > m_LastPos.x + m_LastTileSize.x)
								setNewTile(maths::vec2{ m_LastPos.x + m_LastTileSize.x, m_LastPos.y });
							if (new_pos.y < m_LastPos.y - m_NewTileSize.y)
								setNewTile(maths::vec2{ m_LastPos.x , m_LastPos.y - m_NewTileSize.y });
							if (new_pos.y > m_LastPos.y + m_LastTileSize.y)
								setNewTile(maths::vec2{ m_LastPos.x , m_LastPos.y + m_LastTileSize.y });
						}
						break;
					case Move:
						if (m_Keyboard->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
							moveTiles(maths::vec2{ -pmx, pmy } -maths::vec2{ -mx, my });
						//if (m_SelectedTile != -1 && m_Keyboard->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
						//{
						//	(*m_Tiles)[m_SelectedTile].position += maths::vec2{ -pmx, pmy } -maths::vec2{ -mx, my };
						//	m_MapLayer->m_Tiles[m_SelectedTile].move(maths::vec2{ -pmx, pmy } -maths::vec2{ -mx, my });
						//}
						break;
					case Delete:
						if (m_Keyboard->isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
						{
							deleteTile(mx, my);
						}
						break;
					default: break;
					}
				}
				return true;
			}
			bool is_window_contains(maths::vec2 mouse_pos) const override { return true; }
			void addTile(const SRL::TileData& tdb);

			void setNewTile(maths::vec2 ms_pos);

			int findCurrentTile(float mx, float my)
			{
				maths::vec2 ms_pos = maths::vec2{ mx, my } +maths::vec2{ -900 + m_Shift.x * 2,  450 } +m_Cam->getPosition();
				int i = 0;
				for (const auto& tile : *m_Tiles)
				{
					if (maths::Rectangle{ tile.position, m_TilesUv[tile.type].UvSize }.Contains(ms_pos))
					{
						m_LastPos = tile.position - maths::vec2{ -900,  450 } -m_Cam->getPosition() - m_Shift;
						m_LastTileSize = m_TilesUv[tile.type].UvSize;
						return i;
					}
					i++;
				}
				return -1;
			}

			void moveTile(int tile_to_move, maths::vec2 step);

			void moveTiles(maths::vec2 step);

			void deleteTile(unsigned int id);

			void deleteTile(float mx, float my);

			void loadFootholdsLayer();
		};
	}
}
