#pragma once
#include <map>
#include <vector>
#include <fstream>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>

namespace hiraeth {
	namespace item {

		struct ItemData
		{
			std::string item_name;
			template<class A> void serialize(A& ar) {
				ar(item_name);
			}
		};
		
		class ItemDataManager
		{
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