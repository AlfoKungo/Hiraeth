#include "item_data_manager.h"

namespace hiraeth {
	namespace item {

			std::map<unsigned int, ItemData> ItemDataManager::m_ItemData;

			const ItemData& ItemDataManager::Get(unsigned int index) 
			{
				if (m_ItemData.find(index) == m_ItemData.end())
					m_ItemData[index] = deserialize_item_data(index);
				return m_ItemData[index]; 
			}

			ItemData ItemDataManager::deserialize_item_data(unsigned int item_index)
			{
				std::ifstream file("item.data");
				cereal::BinaryInputArchive iarchive(file);
				int start_of_data;
				file.seekg(sizeof(int) * (item_index - 1));
				iarchive(start_of_data);
				file.seekg(start_of_data);
				ItemData item_data;
				iarchive(item_data);
				return item_data;
			}
	}
}
