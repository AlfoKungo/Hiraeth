#include "item_data_manager.h"

namespace hiraeth {
	namespace item {

			std::map<unsigned int, SRL::ItemData> ItemDataManager::m_ItemData;

			const SRL::ItemData& ItemDataManager::Get(unsigned int index) 
			{
				if (m_ItemData.find(index) == m_ItemData.end())
					m_ItemData[index] = SRL::deserial<SRL::ItemData>("serialized/item.data", (index));
				return m_ItemData[index]; 
			}
	}
}
