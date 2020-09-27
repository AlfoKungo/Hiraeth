#pragma once
#include "ui_window.h"
#include "graphics/sprite.h"
#include "graphics/texture_manager.h"
#include "game/net_chars/net_char_manager.h"
#include "game/character_graphics.h"
#include "ui_basic/ui_button_basic.h"

namespace hiraeth {
	namespace ui {
		const float UI_TRADE_WIDTH{ 526.0f }, UI_TRADE_HEIGHT{ 472 }, UI_TRADE_TITLEBAR_WIDTH{ 270 };
		class UiTrade : public UiWindow
		{
		const std::string UI_TRADE_WINDOW_NAME{ "trade" };
		const std::string UI_TRADE_BT_TRADE{ "BtTrade" };
		const std::string UI_TRADE_BT_CANCEL{ "BtReset" };
			std::vector<std::unique_ptr<graphics::Renderable>> m_MeRends;
			std::vector<std::unique_ptr<graphics::Renderable>> m_OtherRends;
			//std::map<SRL::EquipItemType, item::EquipItem*>* m_Equips;
			graphics::TGroup<ui::UiButton> m_Buttons;
			//maths::mat4 m_TransformationMatrix;

			std::map<unsigned int, item::EquipItem*> m_MeItems;
			std::map<unsigned int, item::EquipItem*> m_OtherItems;
		public:
			bool m_IsTrading{ false };

		public:
			UiTrade(maths::vec2 pos)
				: UiWindow(maths::Rectangle{ pos, {UI_TRADE_WIDTH, UI_TRADE_HEIGHT} }, UiKey::none)
				//m_TransformationMatrix{}
				//m_ButtonTrade{ {300,300}, UI_TRADE_WINDOW_NAME, UI_TRADE_BT_TRADE, std::bind(&UiTrade::acceptTrade, this)},
				//m_ButtonCancel{ {300,300}, UI_TRADE_WINDOW_NAME, UI_TRADE_BT_TRADE, std::bind(&UiTrade::cancelTrade, this)}
			{
				m_BackgroundGroup->add(new graphics::Sprite(maths::vec2{ 0,0 }, graphics::TextureManager::Load("Assets/UI/Trade/TradingRoom.backgrnd.png")));
				m_BackgroundGroup->add(new graphics::Sprite(maths::vec2{ 6, 6 }, graphics::TextureManager::Load("Assets/UI/Trade/TradingRoom.backgrnd2.png")));
				m_BackgroundGroup->add(new graphics::Sprite(maths::vec2{ 10,10 }, graphics::TextureManager::Load("Assets/UI/Trade/TradingRoom.backgrnd3.png")));

				//m_Buttons.add(new UiButtonBasic{ {300,300}, "trade", "BtTrade", std::bind(&UiTrade::acceptTrade, this) });
				m_Buttons.add(new UiButtonBasic{ {104,391}, UI_TRADE_WINDOW_NAME, UI_TRADE_BT_TRADE, std::bind(&UiTrade::acceptTrade, this) });
				m_Buttons.add(new UiButtonBasic{ {104,371}, UI_TRADE_WINDOW_NAME, UI_TRADE_BT_CANCEL, std::bind(&UiTrade::cancelTrade, this) });
				m_BackgroundGroup->add_ref(&m_Buttons);
			}
			//void openTrade(unsigned int char_id)
			void openTrade(std::map<SRL::EquipItemType, graphics::Sprite>* main_char_equips, game::NetChar* net_char)
			{
				m_IsTrading = true;
				game::FillCharacterStandData(m_MeRends);
				game::FillCharacterStandData(m_OtherRends);
				is_to_draw = true;
				is_to_update = true;
				for (auto& rend : m_MeRends)
				{
					rend->setPosition(rend->getPosition() + maths::vec2{ 190, 358 });
					//m_ForegroundGroup->add_ref(rend.get());
				}
				for (auto& [key, rend] : *main_char_equips)
				{
					m_MeRends.push_back(std::make_unique<graphics::Sprite>(maths::vec2{ 196,397 }, rend.getOrigin(), rend.getTexture()));
					//graphics::Sprite(maths::vec2{ 100,100 }, rend.getTexture());
					//rend.setPosition({ 100,100 });
				}
				for (auto& [key, rend] : net_char->m_Equips)
					m_OtherRends.push_back(std::make_unique<graphics::Sprite>(maths::vec2{ 6,39 }, rend.getOrigin(), rend.getTexture()));
				for (auto& rend : m_OtherRends)
					rend->setPosition(rend->getPosition() + maths::vec2{ 0, 358 });
			}
			void closeTrade()
			{
				is_to_draw = false;
				is_to_update = false;
				m_MeRends.clear();
				m_OtherRends.clear();
				m_ForegroundGroup->clear();
				m_MeItems.clear();
				m_OtherItems.clear();
			}
			void draw(graphics::Renderer* renderer) const override
			{
				UiWindow::draw(renderer);
				renderer->push(m_Group.getTransform());
				for (auto& rend : m_MeRends)
				{
					//rend->setPosition(rend->getPosition() + maths::vec2{190, 358});
					rend->draw(renderer);
				}
				//renderer->pop();
				//renderer->push(m_Group.getTransform() * maths::mat4::Translate(maths::vec3( maths::vec2{86, 0}))*
				//	maths::mat4::Scale(maths::vec3(-1, 1, 1)) );
				renderer->push(maths::mat4::Translate(maths::vec3(maths::vec2{ 86, 0 })) *
					maths::mat4::Scale(maths::vec3(-1, 1, 1)));
				for (const auto& rend : m_OtherRends)
				{
					//rend->setPosition(rend->getPosition() + maths::vec2{190, 358});
					rend->draw(renderer);
				}
				////renderer->pop();
				renderer->pop();
				renderer->pop();
			}
			void update() override
			{
				UiWindow::update();
				for (auto& rend : m_MeRends)
					rend->update();
				for (auto& rend : m_OtherRends)
					rend->update();
			}
			//void setEquips(std::map<SRL::EquipItemType, item::EquipItem*>* equips) { m_Equips = equips; }
			void mouse_left_clicked(maths::vec2 mouse_pos) override
			{
				//auto mouse_pos = view::Camera::mouse_to_screen_position(mousePos);
				for (auto& button : m_Buttons.m_Renderables)
				{
						if (button->isContains(mouse_pos))
						{
							button->onClick(mouse_pos);
							//if (!nextDialog())
							//	m_UiQuests->setQuestAsActive(0);
							//return true;
						}
				}
			}
			void mouse_left_released(maths::vec2 mousePos) override {}
			void mouse_moved(float mx, float my, maths::vec2 mousePos) override 
			{
				for (auto& button : m_Buttons.m_Renderables)
				{
					//button->onMove(rel_mouse_pos);
					button->onMove(mousePos);
				}
				// if (m_HoldItem.second != nullptr)
				for (auto& item : m_MeItems)
					item.second->setDrawDetails(false);
				for (auto& item : m_OtherItems)
					item.second->setDrawDetails(false);

				auto item = m_MeItems.find(getPosIndexByMouseIndex(mousePos));
				if (item != m_MeItems.end())
					(item->second)->setDrawDetails(true);

				item = m_OtherItems.find(getPosIndexByMouseIndex(mousePos+ maths::vec2{ 140,0 }) );
				if (item != m_OtherItems.end())
					(item->second)->setDrawDetails(true);
			}

			void mouse_right_clicked(maths::vec2 mousePos) override {}
			maths::Rectangle getTitlebar() const override
			{
				return maths::Rectangle(0, m_WindowSize.y - TITLE_BAR_SIZE, UI_TRADE_TITLEBAR_WIDTH, TITLE_BAR_SIZE);
			}

			unsigned int getPosIndexByMouseIndex(maths::vec2 mouse_pos)
			{
				maths::vec2 box_rltv_pos = mouse_pos - maths::vec2{ 148, 214 };
				unsigned int row = (box_rltv_pos.x) / 37;
				unsigned int line = (box_rltv_pos.y) / 34;
				if (row > 2 || line > 2 || box_rltv_pos.x <0|| box_rltv_pos.y <0)
					return 10;
				return row + line * 3;
			}
			void addItem(unsigned int loc, SRL::EquipDbStruct equip_info, bool is_me)
			{
				EquipItem* item = new EquipItem(equip_info, ItemDataManager::GetEquip(equip_info.equip_id));
				if (is_me)
				{
					item->setPosition(getPosByLoc(loc));
					m_MeItems.emplace(std::make_pair(loc, item));
					m_ForegroundGroup->add(m_MeItems[loc]);
				}
				else
				{
					item->setPosition(getPosByLoc(loc) + maths::vec2{ -138, 0 });
					m_OtherItems.emplace(std::make_pair(loc, item));
					m_ForegroundGroup->add(m_OtherItems[loc]);
				}
			}
			void playerAccepted(bool is_me)
			{
				auto pos = maths::vec2{10,210 };
				if (is_me)
					pos += maths::vec2{ 139, 0 };
				m_ForegroundGroup->add(new graphics::Sprite(pos, 113, 113, 0xaaffffff));
			}

			maths::vec2 getPosByLoc(unsigned int loc)
			{
				return maths::vec2{ float(loc % 3) * 39 + 149, float(loc / 3) * 37 + 213 };
			}

			void acceptTrade()
			{
				NetworkManager::Instance()->sendAcceptTrade();
			}
			void cancelTrade()
			{
				NetworkManager::Instance()->sendCancelTrade();
			}
		};
	}
}
