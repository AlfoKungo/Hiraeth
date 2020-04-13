#pragma once
#include <map>
#include <vector>
#include <fstream>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include "UI/ui_inventory.h"
#include "srl/deserial.h"

namespace hiraeth {
	namespace item {
		
		class ItemDataManager
		{
		private:
			static std::map<unsigned int, SRL::UseItemData> m_ItemData;
			static std::map<unsigned int, SRL::EquipItemData> m_EquipData;
		public:
			static const SRL::UseItemData& Get(unsigned int index);
			static const SRL::EquipItemData& GetEquip(unsigned int index);

		private:
			ItemDataManager();
		};
	}
}