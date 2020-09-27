#pragma once
#include "ui_window.h"
#include "graphics/sprite.h"
#include "graphics/texture_manager.h"
#include "ui_basic/ui_button_basic.h"

namespace hiraeth {
	namespace ui {
		const float UI_SHOP_WIDTH{ 508.0f }, UI_SHOP_HEIGHT{ 505 };
		class UiShop : public UiWindow
		{
			//std::vector<std::unique_ptr<graphics::Renderable>> m_SellItems;
			std::map<unsigned int, item::ItemHold*> m_SellItems;
		public:
			UiShop(maths::vec2 pos)
				: UiWindow(maths::Rectangle{pos, {UI_SHOP_WIDTH, UI_SHOP_HEIGHT}}, UiKey::none)

			{
				m_BackgroundGroup->add(new graphics::Sprite(maths::vec2{ 0,0 }, graphics::TextureManager::Load("Assets/UI/Shop/Shop2.backgrnd.png")));
				m_BackgroundGroup->add(new graphics::Sprite(maths::vec2{ 6, 6 }, graphics::TextureManager::Load("Assets/UI/Shop/Shop2.backgrnd2.png")));
				m_BackgroundGroup->add(new graphics::Sprite(maths::vec2{ 7,10 }, graphics::TextureManager::Load("Assets/UI/Shop/Shop2.backgrnd3.png")));
				//setActivityState(true);
			}
			void draw(graphics::Renderer* renderer) const override
			{
				UiWindow::draw(renderer);
				//for (auto& rend : m_)
			}
			void update() override
			{
				UiWindow::update();
				//for (auto& [key, rend] : m_SellItems)
				//	rend->update();
			}

			virtual void mouse_left_clicked(maths::vec2 mousePos) {}
			virtual void mouse_left_released(maths::vec2 mousePos) {}
			virtual void mouse_right_clicked(maths::vec2 mousePos)
			{
				auto num = getPosIndexByMouseIndex(mousePos);
				if (num < m_SellItems.size())
					NetworkManager::Instance()->sendShopBuyItem(num);
			}
			virtual void mouse_moved(float mx, float my, maths::vec2 mousePos) 
			{
				for (auto& [key, item] : m_SellItems)
					item->setDrawDetails(false);

				auto item = m_SellItems.find(getPosIndexByMouseIndex(mousePos));
				if (item != m_SellItems.end())
					(item->second)->setDrawDetails(true, mousePos + maths::vec2{ -18, -20 });
			}
			unsigned int getPosIndexByMouseIndex(maths::vec2 mouse_pos)
			{
				//maths::vec2 box_rltv_pos = mouse_pos - maths::vec2{ 9, 345 };
				maths::vec2 box_rltv_pos = mouse_pos - maths::vec2{ 9, 387 };
				unsigned int row = (box_rltv_pos.x) / 243;
				unsigned int line = (-box_rltv_pos.y) / 42;
				if (row > 0 || line > 9 || box_rltv_pos.x < 0|| box_rltv_pos.y > 0)
					return 10;
				return line;
			}
			void openShop(const std::vector<SRL::MerchantItemSellData>& sell_items)
			{
				unsigned int i = 0;
				for (const auto& item_info : sell_items)
				{
					if (item_info.item_type == network::EQUIP_ITEM)
					{
						EquipItem* item = new EquipItem(SRL::EquipDbStruct{}, ItemDataManager::GetEquip(item_info.item_id));
						item->setPosition(maths::vec2{ 11.0f, 346.0f - 40 * i });
						m_SellItems.emplace(std::make_pair(i, item));
						m_ForegroundGroup->add(item);
					}
					else
						if (item_info.item_type == network::USE_ITEM)
						{
							ItemHold* item = new UseItem(item_info.item_id,
								ItemDataManager::Get(item_info.item_id), 1);
							item->setPosition(maths::vec2{ 11.0f, 346.0f - 42 * i });
							m_SellItems.emplace(std::make_pair(i, item));
							m_ForegroundGroup->add(item);
						}

					m_ForegroundGroup->add(new graphics::Label("arial", 14, std::to_string(item_info.price) + " niggas", maths::vec2{ 50.0f, 350.0f - 42 * i }, 0xff000000));
					i++;
				}
				setActivityState(true);
			}
		};
	}
}
