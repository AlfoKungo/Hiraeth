#pragma once
#include <utility>
#include "keyboard/keyboard.h"

namespace hiraeth {
	namespace editor {
		class MapController : public input::MouseEvent, public input::KeyboardEvent
		{
			input::Keyboard * m_Keyboard;
			maths::vec2 m_Size;
			maths::vec2 m_Shift;
			graphics::Renderable* m_Cam;
			unsigned int m_TileIndex{};
			std::vector<SRL::TileUv> m_TilesUv;
			std::vector<SRL::TileData>* m_Tiles;
			unsigned int m_RandBegin{ 0 }, m_RandRange{ 6 };
			bool m_ShiftPressed{ false };
			int m_SelectedTile{ -1 };
			map::MapLayer * m_MapLayer;
			maths::vec2 m_LastPos{ 0 }, m_TileSize{0};
			bool m_MouseHold{ false};
			struct CamControl
			{
				bool left{ false }, right{ false };
			};
		public:
			MapController(std::vector<SRL::TileUv> tiles_data, map::MapLayer * map_layer, 
				std::vector<SRL::TileData>* tiles,
				input::Keyboard* keyboard, maths::vec2 size, graphics::Renderable* cam)
				: m_Keyboard{keyboard},
			m_Size(size),
			m_Cam(cam),
			m_TilesUv(std::move(tiles_data)),
			m_Tiles{tiles},
			m_MapLayer{map_layer}
			{
				m_Keyboard->registerToMouse(this);
				m_Keyboard-> registerToKey(GLFW_KEY_LEFT_SHIFT, 0, this);
			}

			void update()
			{
				if (m_Keyboard->isKeyPressed(GLFW_KEY_RIGHT) || m_Keyboard->isKeyPressed(GLFW_KEY_D))
					m_Cam->setPosition(m_Cam->getPosition() + maths::vec2{ 10.0f, 0.0f });
				if (m_Keyboard->isKeyPressed(GLFW_KEY_LEFT) || m_Keyboard->isKeyPressed(GLFW_KEY_A))
					m_Cam->setPosition(m_Cam->getPosition() - maths::vec2{ 10.0f, 0.0f });
				if (m_Keyboard->isKeyPressed(GLFW_KEY_UP) || m_Keyboard->isKeyPressed(GLFW_KEY_W))
					m_Cam->setPosition(m_Cam->getPosition() + maths::vec2{ 0.0f, 10.0f });
				if (m_Keyboard->isKeyPressed(GLFW_KEY_DOWN) || m_Keyboard->isKeyPressed(GLFW_KEY_S))
					m_Cam->setPosition(m_Cam->getPosition() - maths::vec2{ 0.0f, 10.0f });
			}

			void ButtonClicked(input::Key control) override
			{
				if (control == 0)
					m_ShiftPressed = true;
			}

			void ButtonReleased(input::Key control) override
			{
				if (control == 0)
				{
					m_ShiftPressed = false;
					m_SelectedTile = -1;
				}
			}

			bool leftButtonClicked(float mx, float my) override
			{
				if (m_ShiftPressed)
				{
					m_SelectedTile = findCurrentTile(mx, -my);
				}
				else
				{
					m_MouseHold = true;
					if (findCurrentTile(mx, -my) == -1)
					{
						setNewTile(maths::vec2{ mx, -my });
						//SRL::TileData td{ maths::vec2{mx - 900, -my + 450} +m_Cam->getPosition() + m_Shift, 1.0f, m_TileIndex };
						//m_LastPos = maths::vec2{ mx - 900, -my + 450 } +m_Cam->getPosition() + m_Shift;
						//m_Tiles->push_back(td);
						//m_MapLayer->m_Tiles.emplace_back(td);
					}
				}
				return true;
			}
			bool leftButtonReleased(float mx, float my) override
			{
				m_MouseHold = false; 
				//m_SelectTile = false;
					m_SelectedTile = -1;
				return true;
			}
			bool rightButtonClicked(float mx, float my) override
			{
				if (m_ShiftPressed)
				{
					m_SelectedTile = findCurrentTile(mx, -my);
					m_Tiles->erase(m_Tiles->begin() + m_SelectedTile);
					m_MapLayer->m_Tiles.erase(m_MapLayer->m_Tiles.begin() + m_SelectedTile);
					m_SelectedTile = -1;
				}
				else {
					maths::vec2 rel_pos{ mx - 50, 450 + 160 - my };
					int i = 0;
					for (const auto& tile_data : m_TilesUv)
					{
						if (maths::Rectangle{ tile_data.UvPos, tile_data.UvSize }.Contains(rel_pos))
						{
							m_TileIndex = i;
							m_TileSize = tile_data.UvSize;
							m_Shift = maths::vec2{ tile_data.UvSize.x / 2.0f, -tile_data.UvSize.y / 2.0f };
							return true;
						}
						i++;
					}
				}
				return true;
			}
			bool mouseMove(float pmx, float pmy, float mx, float my)  override
			{
				if (m_ShiftPressed)
				{
					if (m_SelectedTile != -1)
					{
						(*m_Tiles)[m_SelectedTile].position += maths::vec2{ -pmx, pmy } - maths::vec2{ -mx, my };
						m_MapLayer->m_Tiles[m_SelectedTile].move(maths::vec2{ -pmx, pmy } -maths::vec2{ -mx, my });
					}
				}
				else
					if (m_MouseHold)
					{
						//maths::vec2 new_pos = maths::vec2{ mx - 900, -my + 450 } +m_Cam->getPosition() + m_Shift;
						maths::vec2 new_pos = maths::vec2{ mx, -my };
						if (new_pos.x < m_LastPos.x - m_TileSize.x)
							setNewTile(maths::vec2{ m_LastPos.x - m_TileSize.x, m_LastPos.y });
						if (new_pos.x > m_LastPos.x + m_TileSize.x)
							setNewTile(maths::vec2{ m_LastPos.x + m_TileSize.x, m_LastPos.y });
						if (new_pos.y < m_LastPos.y - m_TileSize.y)
							setNewTile(maths::vec2{ m_LastPos.x , m_LastPos.y - m_TileSize.y });
						if (new_pos.y > m_LastPos.y + m_TileSize.y)
							setNewTile(maths::vec2{ m_LastPos.x , m_LastPos.y + m_TileSize.y });
					}
				return true;
			}
			bool is_window_contains(maths::vec2 mouse_pos) const override { return true; }
			void setNewTile(maths::vec2 ms_pos)
			{
				if (findCurrentTile(ms_pos.x, ms_pos.y) == -1)
				{
					//SRL::TileData td{ ms_pos + maths::vec2{ -900,  450} +m_Cam->getPosition() + m_Shift, 1.0f, m_TileIndex };
					SRL::TileData td{ ms_pos + maths::vec2{ -900,  450} +m_Cam->getPosition() + m_Shift, 
						1.0f, m_RandBegin + std::rand() % m_RandRange };
					std::cout << "set Tile in " << td.position << " and type is " << td.type << std::endl;
					m_LastPos = ms_pos;
					m_Tiles->push_back(td);
					m_MapLayer->m_Tiles.emplace_back(td);
				}
			}

			int findCurrentTile(float mx, float my)
			{
				maths::vec2 ms_pos = maths::vec2{ mx, my } +maths::vec2{ -900 + m_Shift.x * 2,  450 } + m_Cam->getPosition();
				int i = 0;
				for (const auto& tile : *m_Tiles)
				{
					if (maths::Rectangle{ tile.position, m_TilesUv[tile.type].UvSize }.Contains(ms_pos))
					{
						m_LastPos = tile.position - maths::vec2{ - 900,  450} - m_Cam->getPosition() - m_Shift;
						return i;
					}
					i++;
				}
				return -1;
			}

		};
	}
}
