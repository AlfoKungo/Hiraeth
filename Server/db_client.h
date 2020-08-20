#pragma once
#include "libpq-fe.h"
#include <string>
#include <iomanip>


namespace hiraeth {
	namespace network {
			inline const char* STATS_ALLOC = "stats_alloc";
			inline const char* SKILLS_ALLOC = "skills_alloc";
			inline const char* INV_EQUIP = "inv_equip";
			inline const char* INV_USE = "inv_use";
			inline const char* INV_SETUP = "inv_setup";
			inline const char* INV_ETC = "inv_etc";
			inline const char* INV_CASH = "inv_cash";
			inline const char* EQUIP_CHAR = "equips_char";
			inline const char* PLAYER_STATE = "player_state";
			inline const char* QUESTS_IN_PROGRESS = "quests_in_prog";
			inline const char* QUESTS_DONE = "quests_done";
		class DbClient
		{
			const std::map<unsigned int, const char*> TABS_MAP{
				{EQUIP_ITEM, INV_EQUIP}, {USE_ITEM, INV_USE},
				{SETUP_ITEM, INV_SETUP},{ETC_ITEM, INV_ETC},{CASH_ITEM, INV_CASH} };
			const char* get_name_by_index(unsigned int tab_index) const { return TABS_MAP.at(tab_index); }
			//int nFields{ 0 };
			PGconn *m_Conn;
			PGresult *m_Res;
		public:
			DbClient()
			{
				m_Conn = PQconnectdb("dbname = test user = postgres password = 12341234");
				if (PQstatus(m_Conn) != CONNECTION_OK)
				{
					fprintf(stderr, "Connection to database failed: %s",
						PQerrorMessage(m_Conn));
				}
			}

			void addPlayer(std::string name)
			{
				std::string command = "INSERT INTO players (name, level, exp, job) VALUES ('" + name + "', 1, 0, 1)";
				m_Res = PQexec(m_Conn, command.c_str());
				if (PQresultStatus(m_Res) != PGRES_COMMAND_OK)
				{
					fprintf(stderr, "INSERT command failed: %s", PQerrorMessage(m_Conn));
					PQclear(m_Res);
				}
				PQclear(m_Res);
				//m_Res = PQexec(m_Conn, "CLOSE players");
				//PQclear(m_Res);
			}

			std::string getNameById(unsigned int id)
			{
				const char *paramValues[1];
				paramValues[0] = "2";
				PGresult   *res;

				res = PQexecParams(m_Conn,
					"SELECT * FROM players WHERE id = $1",
					1,       /* one param */
					NULL,    /* let the backend deduce param type */
					paramValues,
					NULL,    /* don't need param lengths since text */
					NULL,    /* default to all text params */
					1);      /* ask for binary results */

				if (PQresultStatus(res) != PGRES_TUPLES_OK)
				{
					fprintf(stderr, "SELECT failed: %s", PQerrorMessage(m_Conn));
					PQclear(res);
				}

				std::string name{ getString("name") };
				PQclear(res);
				return name;
			}

			unsigned int getIdByAccountName(const std::string& account_name)
			{
				const char *paramValues[1];
				paramValues[0] = account_name.c_str();

				m_Res = PQexecParams(m_Conn,
					"SELECT id FROM accounts WHERE account = $1",
					1,       /* one param */
					NULL,    /* let the backend deduce param type */
					paramValues,
					NULL,    /* don't need param lengths since text */
					NULL,    /* default to all text params */
					1);      /* ask for binary results */

				if (PQresultStatus(m_Res) != PGRES_TUPLES_OK)
				{
					fprintf(stderr, "SELECT failed: %s", PQerrorMessage(m_Conn));
					PQclear(m_Res);
				}

				int id = getInt("id");
				//auto name = getString(PQfnumber(m_Res, "account")),
				//	pass = getString(PQfnumber(m_Res, "password"));
				PQclear(m_Res);
				return id;
			}

			PlayerData getPlayerDataById(unsigned int player_id)
			{
				const char *paramValues[1];
				std::string id_string{ std::to_string(player_id) };
				paramValues[0] = id_string.data();

				m_Res = PQexecParams(m_Conn,
					"SELECT * FROM players WHERE id = $1",
					1,       /* one param */
					NULL,    /* let the backend deduce param type */
					paramValues,
					NULL,    /* don't need param lengths since text */
					NULL,    /* default to all text params */
					1);      /* ask for binary results */

				if (PQresultStatus(m_Res) != PGRES_TUPLES_OK)
				{
					fprintf(stderr, "SELECT failed: %s", PQerrorMessage(m_Conn));
					PQclear(m_Res);
				}

				const unsigned int level = getInt("level"),
					exp = getInt("exp"),
					job = getInt("job"),
					max_hp = getInt("max_hp"),
					hp = getInt("hp"),
					max_mp = getInt("max_mp"),
					mp = getInt("mp");
				std::string player_name{ getString("name") };
				const auto stats_alloc = getDynamicTypeFromBuffer<decltype(PlayerData::stats_alloc)>(STATS_ALLOC);
				const auto skills_alloc = getDynamicTypeFromBuffer<decltype(PlayerData::skills_alloc)>(SKILLS_ALLOC);
				const auto inv_equip = getDynamicTypeFromBuffer<decltype(PlayerData::inv_equip)>(INV_EQUIP);
				const auto inv_use = getDynamicTypeFromBuffer<decltype(PlayerData::inv_use)>(INV_USE);
				const auto inv_setup = getDynamicTypeFromBuffer<decltype(PlayerData::inv_setup)>(INV_SETUP);
				const auto inv_etc = getDynamicTypeFromBuffer<decltype(PlayerData::inv_etc)>(INV_ETC);
				const auto inv_cash = getDynamicTypeFromBuffer<decltype(PlayerData::inv_cash)>(INV_CASH);
				const auto equips_char = getDynamicTypeFromBuffer<decltype(PlayerData::equips_char)>(EQUIP_CHAR);
				const auto player_state = getDynamicTypeFromBuffer<decltype(PlayerData::player_hold_state)>(PLAYER_STATE);
				const auto quests_in_progress = getDynamicTypeFromBuffer<decltype(PlayerData::quests_in_progress)>(QUESTS_IN_PROGRESS);
				const auto quests_done = getDynamicTypeFromBuffer<decltype(PlayerData::quests_done)>(QUESTS_DONE);

				PQclear(m_Res);
				PlayerData player_data{ PlayerStats{player_name, level, job, exp, max_hp,
					hp, max_mp, mp}, stats_alloc, skills_alloc , inv_equip, inv_use,
					inv_setup, inv_etc, inv_cash, equips_char, player_state, quests_in_progress, quests_done};
				return player_data;
			}

			template <class T>
			T getDynamicType(unsigned int player_id, const std::string& field)
			{
				const char *paramValues[1];
				std::string id_string{ std::to_string(player_id) };
				paramValues[0] = id_string.data();
				std::string sCommand = "SELECT " + field + " FROM players WHERE id = $1";

				m_Res = PQexecParams(m_Conn,
					sCommand.c_str(),
					1,       /* one param */
					NULL,    /* let the backend deduce param type */
					paramValues,
					NULL,    /* don't need param lengths since text */
					NULL,    /* default to all text params */
					1);      /* ask for binary results */

				if (PQresultStatus(m_Res) != PGRES_TUPLES_OK)
				{
					fprintf(stderr, "SELECT failed: %s", PQerrorMessage(m_Conn));
					PQclear(m_Res);
				}
				auto dt = getDynamicTypeFromBuffer<T>(field.c_str());
				PQclear(m_Res);
				return std::move(dt);
				//return dt;
			}

			void setValue(unsigned int client_id, std::string field, unsigned int new_value)
			{
				std::string command = "UPDATE players SET " + field + " = "
					+ std::to_string(new_value) + " WHERE id = " + std::to_string(client_id);
				m_Res = PQexec(m_Conn, command.c_str());
				if (PQresultStatus(m_Res) != PGRES_COMMAND_OK)
				{
					fprintf(stderr, "INSERT command failed: %s", PQerrorMessage(m_Conn));
					PQclear(m_Res);
				}
				PQclear(m_Res);
			}
			template <class T>
			void setByteArray(unsigned int player_id, std::string field, const T& object, std::string table_name = "players")
			{
				auto[buffer, size] = srl_dynamic_type(object);
				//std::string ref_data{ *buffer, unsigned int(size) };
				std::string ref_data{ *buffer, (unsigned int)(size) };
				const std::string data{ hexStr(*buffer, size) };
				std::string command = "UPDATE " + table_name + " SET " + field +
					" = E'\\\\x" + data + "' WHERE id = " + std::to_string(player_id);
				std::cout << command << std::endl;
				m_Res = PQexec(m_Conn, command.c_str());
				if (PQresultStatus(m_Res) != PGRES_COMMAND_OK)
				{
					fprintf(stderr, "INSERT command failed: %s", PQerrorMessage(m_Conn));
					PQclear(m_Res);
				}
				PQclear(m_Res);
			}
			void setStatsAlloc(unsigned int player_id, const std::vector<unsigned int>& stats_alloc)
			{
				auto[buffer, size] = srl_dynamic_type(stats_alloc);
				std::string ref_data{ *buffer, unsigned int(size) };
				//for (int i = 0; i < size; ++i)
				//	//ss << std::hex << (int)(*buffer)[i];
				//	//data += std::to_string((int)(*buffer)[i]);
				//	std::cout << std::hex << (int)(*buffer)[i];
				////std::string data{ss.str()};
				std::string data{ hexStr(*buffer, size) };
				std::string field = "stats_alloc";
				std::string command = "UPDATE players SET " + field + " = E'\\\\x"
					+ data + "' WHERE id = " + std::to_string(player_id);
				//+ std::to_string(new_value) + " WHERE id = " + std::to_string(client_id);
				std::cout << command << std::endl;
				m_Res = PQexec(m_Conn, command.c_str());
				if (PQresultStatus(m_Res) != PGRES_COMMAND_OK)
				{
					fprintf(stderr, "INSERT command failed: %s", PQerrorMessage(m_Conn));
					PQclear(m_Res);
				}
				PQclear(m_Res);
			}
			
			template <class T>
			unsigned int findAvailableLoc(std::map<unsigned int, T> m) const
			{
				unsigned int val = 0;
				for (const auto& [key, d] : m)
				{
					if (key != val)
						break;
					val++;
				}
				return val;
			}

			unsigned int addItem(unsigned int tab_index, unsigned int player_id, unsigned int item_id)
			{
				const char* tab_name = get_name_by_index(tab_index);
				auto items = getDynamicType<decltype(PlayerData::inv_use)>(player_id, tab_name);
				unsigned int item_loc = 0;
				bool found = false;
				for (auto& [key, alloc] : items)
				{
					if (alloc.item_type_id == item_id)
					{
						alloc.item_amount += 1;
						item_loc = key;
						found = true;
						break;
					}
				}
				if (!found)
				{
					item_loc = findAvailableLoc(items);
					items.insert({ item_loc, { item_id , 1} });
				}
				setByteArray(player_id, INV_USE, items);
				return item_loc;
			}

			unsigned int addEquipInv(unsigned int player_id, unsigned int equip_loc)
			{
				auto inv_equip = getDynamicType<decltype(PlayerData::inv_equip)>(player_id, INV_EQUIP);
				const auto item_loc = findAvailableLoc(inv_equip);
				//inv_equip[item_loc] = equip_loc;
				inv_equip[item_loc] = equip_loc;
				setByteArray(player_id, INV_EQUIP, inv_equip);
				return item_loc;
			}

			void wearEquip(unsigned int player_id,SRL::EquipItemType equip_type, unsigned int equip_loc)
			{
				auto equips_inv = getDynamicType<decltype(PlayerData::inv_equip)>(player_id, INV_EQUIP);
				auto equips_char = getDynamicType<decltype(PlayerData::equips_char)>(player_id, EQUIP_CHAR);
				unsigned int old_equip = 0;
				bool is_old_equip{ false };
				if (equips_char.find(equip_type) != equips_char.end())
				{
					old_equip = equips_char[equip_type];
					is_old_equip = true;
				}
				equips_char[equip_type] = equips_inv[equip_loc];
				equips_inv.erase(equip_loc);
				if (is_old_equip)
					equips_inv[equip_loc] = old_equip;
				setByteArray(player_id, INV_EQUIP, equips_inv);
				setByteArray(player_id, EQUIP_CHAR, equips_char);
			}

			void switchInventoryItems(unsigned int player_id, unsigned int item_loc1, 
				unsigned int item_loc2, unsigned int tab_index)
			{
				if (tab_index == EQUIP_ITEM)
					switchInventoryItems<decltype(PlayerData::inv_equip)>(player_id, item_loc1,
						item_loc2, tab_index);
				else
					switchInventoryItems<decltype(PlayerData::inv_use)>(player_id, item_loc1,
						item_loc2, tab_index);
			}

			template <class T>
			void switchInventoryItems(unsigned int player_id, unsigned int item_loc1, 
				unsigned int item_loc2, unsigned int tab_index)
			{
				const char* tab_name = get_name_by_index(tab_index);
				auto inv = getDynamicType<T>(player_id, tab_name);
				auto item2 = inv.find(item_loc2);
				if (item2 != inv.end())
				{
					auto item1 = inv.find(item_loc1);
					std::swap(item1->second, item2->second);
				}
				else
				{
					auto nh = inv.extract(item_loc1);
					nh.key() = item_loc2;
					inv.insert(move(nh));
				}
				setByteArray(player_id, tab_name, inv);
			}

			void increaseSkillPoints(unsigned int player_id, unsigned int skill_id)
			{
				auto skills_alloc = getDynamicType<decltype(PlayerData::skills_alloc)>(player_id, SKILLS_ALLOC);
				for (auto& alloc : skills_alloc)
				{
					if (alloc.skill_id == skill_id)
						alloc.pts_alloc += 1;
					if (alloc.skill_id == 666)
						alloc.pts_alloc -= 1;
				}
				setByteArray(player_id, SKILLS_ALLOC, skills_alloc);
			}

			void setQuestAsActive(unsigned int player_id, unsigned int quest_id)
			{
				auto quests_in_progress = getDynamicType<decltype(PlayerData::quests_in_progress)>(player_id, QUESTS_IN_PROGRESS);
				quests_in_progress.push_back({ quest_id, 0});
				setByteArray(player_id, QUESTS_IN_PROGRESS, quests_in_progress);
			}

		private:

			int getInt(const char* column_name)
			{
				const int column = PQfnumber(m_Res, column_name);
				char *val_p = PQgetvalue(m_Res, 0, column);
				return ntohl(*reinterpret_cast<uint32_t *>(val_p));
			}
			std::string	getString(const char* column_name)
			{
				const int column = PQfnumber(m_Res, column_name);
				char *name = PQgetvalue(m_Res, 0, column);
				int blen = PQgetlength(m_Res, 0, column);
				printf(" b = (%d bytes) ", blen);
				std::cout << name;

				return std::string{ name };
			}
			template<class T>
			T getDynamicTypeFromBuffer(const char* column_name)
			{
				const int column = PQfnumber(m_Res, column_name);
				char *val_ptr = PQgetvalue(m_Res, 0, column);
				unsigned int len = PQgetlength(m_Res, 0, column);
				std::string data_str{ val_ptr, len };
				return dsrl_dynamic_type<T>(data_str.c_str());
			}
			std::string hexStr(char* data, int len)
			{
				std::stringstream ss;
				ss << std::hex;
				for (int i = 0; i < len; ++i)
					ss << std::setw(2) << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(data[i]));
				return ss.str();
			}

		};
	}
}
