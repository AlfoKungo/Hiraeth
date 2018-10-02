#include "item_data_manager.h"

namespace hiraeth {
	namespace item {

			std::map<unsigned int, SRL::UseItemData> ItemDataManager::m_ItemData;
			std::map<unsigned int, SRL::EquipItemData> ItemDataManager::m_EquipData;

			const SRL::UseItemData& ItemDataManager::Get(unsigned int index) 
			{
				if (m_ItemData.find(index) == m_ItemData.end())
					m_ItemData[index] = SRL::deserial<SRL::UseItemData>("serialized/item.data", (index));
				return m_ItemData[index]; 
			}

			const SRL::EquipItemData& ItemDataManager::GetEquip(unsigned int index)
			{
				if (m_EquipData.find(index) == m_EquipData.end())
					m_EquipData[index] = SRL::deserial<SRL::EquipItemData>("serialized/equip.data", (index));
				return m_EquipData[index]; 
			}
	}
}
