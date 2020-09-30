#include "hrth_pch.h"
#include "map_controller.h"

namespace hiraeth {
	namespace editor {

		void MapController::setModeLabel(EditState new_mode)
		{
			m_EditState = new_mode;
			updateModeLabels();
		}

		void MapController::updateModeLabels()
		{
			switch (m_MainEditState)
			{
			case Tiles:
				m_MainEditStateLabel.setText("Main: Tiles");
				switch (m_EditState)
				{
				case Add:
					m_EditStateLabel.setText("Mode: Add");
					m_SelectionLabel.setText("Adding: {" + std::to_string(m_RandBegin) + "," + std::to_string(m_RandBegin + m_RandRange) + "}");
					break;
				case Move:
					m_EditStateLabel.setText("Mode: Move");
					setSelectionLabel();
					break;
				case Delete:
					m_EditStateLabel.setText("Mode: Delete");
					m_SelectionLabel.setText("Deleting");
					break;
				default:break;
				}
				break;
			case Footholds:
				m_MainEditStateLabel.setText("Main: Footholds");
				break;
			case Portals:
				m_MainEditStateLabel.setText("Main: Portals");
				break;
			case Npcs:
				m_MainEditStateLabel.setText("Main: Npcs");
				break;
			case Mobs:
				m_MainEditStateLabel.setText("Main: Mobs");
				break;
			default:
				break;
			}
		}

		void MapController::setMainModeLabel(MainEditState new_mode)
		{
			m_MainEditState = new_mode;
			updateModeLabels();
		}

		void MapController::setSelectionLabel()
			{
				std::string sel{};
				for (const auto& num : m_SelectionVector)
					sel.append(std::to_string(num) + ", ");
				if (sel.length() > 0)
					sel.erase(sel.end() - 2, sel.end());
				m_SelectionLabel.setText("Selection: {" + sel + "}");
			}

			void MapController::setUvAdd(float mx, float my)
			{
				maths::vec2 rel_pos{mx - 50, 450 - 110 - my};
				unsigned int i = 0;
				for (const auto& tile_data : m_TilesUv)
				{
					if (maths::Rectangle{tile_data.UvPos, tile_data.UvSize}.Contains(rel_pos))
					{
						m_TileIndex = i;
						unsigned int c = 0;
						for (auto j : m_GroupingList)
						{
							if (i < c + j)
							{
								m_RandBegin = c;
								m_RandRange = j;
								m_SelectionLabel.setText("Selection: {" + std::to_string(m_RandBegin) + "," +
								                         std::to_string(m_RandBegin + m_RandRange) + "}");
								break;
							}
							c += j;
						}
						m_NewTileSize = tile_data.UvSize;
						m_Shift = maths::vec2{tile_data.UvSize.x / 2.0f, -tile_data.UvSize.y / 2.0f};
						return;
					}
					i++;
				}
			}

		void MapController::addTile(const SRL::TileData& tdb)
		{
			m_Tiles->push_back(tdb);
			m_MapLayer->m_Tiles.emplace_back(tdb);
		}

		void MapController::setNewTile(maths::vec2 ms_pos)
			{
				if (findCurrentTile(ms_pos.x, ms_pos.y) == -1)
				{
					//SRL::TileData td{ ms_pos + maths::vec2{ -900,  450} +m_Cam->getPosition() + m_Shift, 1.0f, m_TileIndex };
					SRL::TileData td{
						ms_pos + maths::vec2{-900, 450} + m_Cam->getPosition() + m_Shift,
						1.0f, m_RandBegin + std::rand() % m_RandRange
					};
					std::cout << "set Tile in " << td.position << " and type is " << td.type << std::endl;
					m_LastPos = ms_pos;
					addTile(td);
					//m_Tiles->push_back(td);
					//m_MapLayer->m_Tiles.emplace_back(td);
					if (m_RandBegin == 6)
					{
						ms_pos.x += m_TilesUv[6].UvSize.x / 2 - m_TilesUv[8].UvSize.x / 2;
						ms_pos.y += m_TilesUv[6].UvSize.y;
						SRL::TileData tdb{
							ms_pos + maths::vec2{-900, 450} + m_Cam->getPosition() + m_Shift,
							1.0f, 9
						};
						//SRL::TileData td{ ms_pos + maths::vec2{ -900,  450} + m_Cam->getPosition() + m_Shift,
						//	1.0f, m_RandBegin + 2 + std::rand() % m_RandRange };
						std::cout << "set Tile in " << tdb.position << " and type is " << tdb.type << std::endl;
						m_LastPos = ms_pos;
						addTile(tdb);
						//m_Tiles->push_back(tdb);
						//m_MapLayer->m_Tiles.emplace_back(tdb);
					}
				}
			}

			void MapController::moveTile(int tile_to_move, maths::vec2 step)
			{
				(*m_Tiles)[tile_to_move].position += step;
				m_MapLayer->m_Tiles[tile_to_move].move(step);
			}

			void MapController::moveTiles(maths::vec2 step)
			{
				for (const auto& tile_to_move : m_SelectionVector)
				{
					(*m_Tiles)[tile_to_move].position += step;
					m_MapLayer->m_Tiles[tile_to_move].move(step);
				}
			}

			void MapController::deleteTile(unsigned id)
			{
				m_Tiles->erase(m_Tiles->begin() + id);
				m_MapLayer->m_Tiles.erase(m_MapLayer->m_Tiles.begin() + id);
			}

			void MapController::deleteTile(float mx, float my)
			{
				m_SelectedTile = findCurrentTile(mx, -my);
				if (m_SelectedTile != -1)
				{
					std::cout << "delete Tile in " << (*m_Tiles)[m_SelectedTile].position << " and type is " << (*
						m_Tiles)[m_SelectedTile].type << std::endl;
					//m_Tiles->erase(m_Tiles->begin() + m_SelectedTile);
					//m_MapLayer->m_Tiles.erase(m_MapLayer->m_Tiles.begin() + m_SelectedTile);
					deleteTile(m_SelectedTile);
					m_SelectedTile = -1;
				}
			}

			void MapController::loadFootholdsLayer()
			{
				m_FootholdsLayer.clear();
				for (const auto& foothold : m_MapData->FootHolds)
					if (foothold.direction.x == 1.0f)
						m_FootholdsLayer.add(new graphics::Sprite{
							                     foothold.p1,
							                     8.0f, float(abs(foothold.p2.y - foothold.p1.y)), 0xaaaa33bb
						                     });
					else
					{
						const auto height = foothold.p2.y - foothold.p1.y;
						m_FootholdsLayer.add(new graphics::Sprite{
							                     foothold.p1,
							                     foothold.p2.x - foothold.p1.x,
							                     copysignf(1.0, height) * (abs(height) + 8.0f), 0xaaaa33bb
						                     });
					}
			}
	}
}
