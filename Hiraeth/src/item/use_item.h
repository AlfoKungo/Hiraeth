#pragma once
#include "item_hold.h"


namespace hiraeth {
	namespace item {

		class UseItem : public ItemHold
		{
		private:
			SRL::ItemPropertiesMap m_PropertiesMap;
			unsigned int m_Amount{ 0 };
			graphics::Label m_AmountLabel;
		public:
			UseItem(unsigned int item_type_id, SRL::UseItemData item_data,
				unsigned int amount)
				: ItemHold(item_type_id, item_data.info.basic_item_info,
					item_data.texture_data),
				//item_data.texture_data, foot_holds, item_id),
				m_PropertiesMap(item_data.info.item_properties),
				m_Amount(amount),
				m_AmountLabel("arial", 16, amount, {}, 0xff000000)
			{
			}
			void draw(graphics::Renderer* renderer) const override
			{
				ItemHold::draw(renderer);
				m_AmountLabel.draw(renderer);
			}
			
			void setPosition(const maths::vec2& position) override
			{
				ItemHold::setPosition(position);
				m_AmountLabel.setPosition(getPosition()+ maths::vec2{25, 0});
			}
			unsigned int getAmount() { return m_Amount; }
			void increaseAmount(int add)
			{
				setAmount(m_Amount + add);
			}
			void setAmount(unsigned int new_amount)
			{
				m_Amount = new_amount;
				m_AmountLabel.setText(m_Amount);
			}
			void update() override
			{
				ItemHold::update();
			}

			SRL::ItemPropertiesMap * getItemProperties() { return &m_PropertiesMap; }
		};
	}
}