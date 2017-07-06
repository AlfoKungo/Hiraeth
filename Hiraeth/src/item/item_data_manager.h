#pragma once
#include <map>
#include <vector>
#include <fstream>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include "item.h"
#include "UI/ui_inventory.h"

namespace hiraeth {
	namespace item {
		
		class ItemDataManager
		{
		public:
			std::map<item::Tab, std::unique_ptr<graphics::Group>> m_LayerItems;
		private:
			static std::map<unsigned int, ItemData> m_ItemData;
		public:
			static const ItemData& Get(unsigned int index);

		private:
			static ItemData deserialize_item_data(unsigned int item_index);

		private:
			ItemDataManager();
		};
	}
}