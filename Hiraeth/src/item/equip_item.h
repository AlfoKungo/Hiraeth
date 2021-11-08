#pragma once
#include "srl/equip_item_data.h"
#include "item_hold.h"


namespace hiraeth {
	namespace item {

		class EquipItem : public ItemHold
		{
		private:
			//unsigned int m_EquipId;
			SRL::EquipPropertiesMap m_EquipPropertiesMap;
			SRL::EquipItemType m_EquipType;
		public:
			SRL::EquipTexturesMap m_Textures;

			EquipItem(unsigned int equip_id, SRL::EquipItemData item_data)
				: EquipItem(SRL::EquipDbStruct{equip_id, createDefaultProperties(equip_id)}, item_data)
			{

			}
			EquipItem(SRL::EquipDbStruct equip_info, SRL::EquipItemData item_data)
				//const std::vector<physics::FootHold>& foot_holds, unsigned int item_id)
				: ItemHold(equip_info.equip_id, item_data.info.item_info, item_data.icon_texture),
				//m_EquipPropertiesMap(item_data.info.equip_item_properties),
				m_EquipPropertiesMap(equip_info.equip_item_properties),
				m_EquipType(item_data.info.equip_item_type),
				m_Textures(item_data.textures)
			{
				for (const auto& [key, val] : m_EquipPropertiesMap)
				{
					switch (key)
					{
					case SRL::ReqLvl:
						break;
					case SRL::ReqStr:
						break;
					case SRL::ReqDex:
						break;
					case SRL::ReqLuk:
						break;
					case SRL::ReqInt:
						break;
					case SRL::AttackPower:
						m_DetailsBoxLabelContent.addLine("Attack Power: " + std::to_string(std::get<int>(val)));
						break;
					case SRL::StrInc:
						m_DetailsBoxLabelContent.addLine("Str: " + std::to_string(std::get<int>(val)));
						break;
					case SRL::DexInc:
						m_DetailsBoxLabelContent.addLine("Dex: " + std::to_string(std::get<int>(val)));
						break;
					case SRL::LukInc:
						m_DetailsBoxLabelContent.addLine("Luk: " + std::to_string(std::get<int>(val)));
						break;
					case SRL::IntInc:
						m_DetailsBoxLabelContent.addLine("Int: " + std::to_string(std::get<int>(val)));
						break;
					case SRL::MaxHpInc: break;
					case SRL::MaxMpInc: break;
					default:;
					}
				}
			}

			SRL::EquipItemType getItemType() const { return m_EquipType; }
			//const SRL::EquipPropertiesMap& getProperties() const { return m_EquipPropertiesMap; }
			SRL::EquipPropertiesMap& getProperties() { return m_EquipPropertiesMap; }
			SRL::EquipPropertiesMap createDefaultProperties(unsigned int equip_id)
			{
				auto StatsForEquip = SRL::deserial<SRL::EquipItemData>(DF_EQUIP, equip_id).info.equip_props;
				SRL::EquipPropertiesMap props{};
				for (auto [stat_type, default, _plus_minus] : StatsForEquip)
				{
					auto val = default;
					props.emplace(stat_type, val);
				}
				return props;
			}

		};
	}
}
