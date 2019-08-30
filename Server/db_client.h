#pragma once
#include "libpq-fe.h"
#include <string>
#include <iomanip>


namespace hiraeth {
	namespace network {
		class DbClient
		{

			//int nFields{ 0 };
			PGconn *m_Conn;
			PGresult   *m_Res;
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
				const auto stats_alloc = getDynamicTypeFromBuffer<decltype(PlayerData::stats_alloc)>("stats_alloc");
					//auto stats_alloc = getDynamicType<std::vector<unsigned int>>(PQfnumber(m_Res, "stats_alloc")),
				const auto skills_alloc = getDynamicTypeFromBuffer<decltype(PlayerData::skills_alloc)>("skills_alloc");
				std::string player_name{ getString("name") };

				PQclear(m_Res);
				PlayerData player_data{ PlayerStats{player_name, level, job, exp, max_hp, hp, max_mp, mp}, 
					stats_alloc, skills_alloc };
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
				auto dt = getDynamicTypeFromBuffer<T>("skills_alloc");
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
			void setByteArray(unsigned int player_id, std::string field, const T& object)
			{
				auto[buffer, size] = srl_dynamic_type(object);
				//std::string ref_data{ *buffer, unsigned int(size) };
				std::string ref_data{ *buffer, (unsigned int)(size) };
				const std::string data{ hexStr(*buffer, size) };
				std::string command = "UPDATE players SET " + field + " = E'\\\\x"
					+ data + "' WHERE id = " + std::to_string(player_id);
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

			void increaseSkillPoints(unsigned int player_id, unsigned int skill_id)
			{
				auto skills_alloc = getDynamicType<decltype(PlayerData::skills_alloc)>(player_id, "skills_alloc");
				for (auto& alloc : skills_alloc)
				{
					if (alloc.skill_id == skill_id)
						alloc.pts_alloc += 1;
					if (alloc.skill_id == 666)
						alloc.pts_alloc -= 1;
				}
				setByteArray(player_id, "skills_alloc", skills_alloc);
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
