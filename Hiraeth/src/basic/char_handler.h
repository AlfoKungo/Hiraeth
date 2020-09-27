#pragma once
#include <string>
#include "srl/equip_item_data.h"
#include "graphics/sprite.h"

namespace hiraeth
{
	class CharHandler
	{
	public:
		virtual ~CharHandler() = default;
		virtual void writeSay(std::string say_msg) = 0;
		virtual void wearItem(SRL::EquipItemType item_type) = 0;
		virtual void unWearItem(SRL::EquipItemType item_type) = 0;
		virtual std::map<SRL::EquipItemType, graphics::Sprite>* getEquips() = 0;
	};

	class CharManager
	{
	private:
		static CharHandler * _Instance;
	public:
		static void setChar(CharHandler * instance)
		{
			_Instance = instance;
		}
		static CharHandler * Instance()
		{
			return _Instance;
		}
	};
}
