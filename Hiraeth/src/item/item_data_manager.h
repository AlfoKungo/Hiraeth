#pragma once
#include <map>
#include <vector>
#include <fstream>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include "item.h"
#include "UI/ui_inventory.h"
#include "srl/deserial.h"

namespace hiraeth {
	namespace item {
		
		class ItemDataManager
		{
		private:
			static std::map<unsigned int, SRL::ItemData> m_ItemData;
		public:
			static const SRL::ItemData& Get(unsigned int index);

		private:
			ItemDataManager();
		};
	}
}