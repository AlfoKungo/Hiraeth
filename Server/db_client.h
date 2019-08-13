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

				char       *bptr;

				int loc = PQfnumber(res, "name");
				/* Get the field values (we ignore possibility they are null!) */
				bptr = PQgetvalue(res, 0, loc);
				int         blen;
				blen = PQgetlength(res, 0, loc);
				printf(" b = (%d bytes) ", blen);
				std::cout << bptr;
				for (int j = 0; j < blen; j++)
					printf("\\%03o", bptr[j]);

				PQclear(res);
				std::string name{ bptr };
				return name;
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

				int level = getInt(PQfnumber(m_Res, "level")),
					exp = getInt(PQfnumber(m_Res, "exp")),
					job = getInt(PQfnumber(m_Res, "job")),
					hp = getInt(PQfnumber(m_Res, "hp")),
					//id = getInt(PQfnumber(m_Res, "id")),
					mp = getInt(PQfnumber(m_Res, "mp"));
				//std::string dd = PlayerData::stats_alloc;
				auto stats_alloc = getDynamicType<decltype(PlayerData::stats_alloc)>(PQfnumber(m_Res, "stats_alloc")),
				//auto stats_alloc = getDynamicType<GET_TYPE_OF(&PlayerData::stats_alloc)>(PQfnumber(m_Res, "stats_alloc")),
				//auto stats_alloc = getDynamicType<std::vector<unsigned int>>(PQfnumber(m_Res, "stats_alloc")),
					skills_alloc = getDynamicType<std::vector<unsigned int>>(PQfnumber(m_Res, "skills_alloc"));
				std::string player_name{ getString(PQfnumber(m_Res, "name")) };

				PQclear(m_Res);
				//PlayerData player_data{ player_name, level, job, exp, hp, mp, {}, {} };
				PlayerData player_data{ player_name, level, job, exp, hp, mp, stats_alloc, skills_alloc };
				return player_data;
			}

			unsigned int getIdByAccountName(std::string account_name)
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

				int id = getInt(PQfnumber(m_Res, "id"));
				//auto name = getString(PQfnumber(m_Res, "account")),
				//	pass = getString(PQfnumber(m_Res, "password"));
				PQclear(m_Res);
				return id;
			}

			void updateValue(unsigned int client_id, std::string field, unsigned int new_value)
			{
				//std::string command = "INSERT INTO players (name, level, exp, job) VALUES ('" + name + "', 1, 0, 1)";
				std::string command = "UPDATE players SET " + field + " = "
					+ std::to_string(new_value) + " WHERE id = " + std::to_string(client_id);
				//+ std::to_string(new_value) + " WHERE id = " + std::to_string(client_id);
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

			std::vector<unsigned int> getStatsAlloc(unsigned int player_id)
			{
				const char *paramValues[1];
				std::string id_s{ std::to_string(player_id) };
				paramValues[0] = id_s.c_str();

				m_Res = PQexecParams(m_Conn,
					//"SELECT * FROM players WHERE id = $1",
					"SELECT stats_alloc FROM players WHERE id = $1",
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

				unsigned int column = PQfnumber(m_Res, "stats_alloc");
				char *val_ptr = PQgetvalue(m_Res, 0, column);
				unsigned int len = PQgetlength(m_Res, 0, column);
				std::string data_str{ val_ptr, len };
				PQclear(m_Res);
				return dsrl_dynamic_type<std::vector<unsigned int>>(data_str.c_str());
			}

		private:

			int getInt(int column)
			{
				char *val_p = PQgetvalue(m_Res, 0, column);
				return ntohl(*reinterpret_cast<uint32_t *>(val_p));
			}
			std::string	getString(int column)
			{
				char *name = PQgetvalue(m_Res, 0, column);
				int blen = PQgetlength(m_Res, 0, column);
				printf(" b = (%d bytes) ", blen);
				std::cout << name;

				return std::string{ name };
			}
			template<class T>
			T getDynamicType(int column)
			{
				char *val_ptr = PQgetvalue(m_Res, 0, column);
				unsigned int len = PQgetlength(m_Res, 0, column);
				std::string data_str{ val_ptr, len };
				return dsrl_dynamic_type<T>(data_str.c_str());
				//return dsrl_dynamic_type<std::vector<unsigned int>>(data_str.c_str());
			}
			std::string hexStr(char* data, int len)
			{
				std::stringstream ss;
				ss << std::hex;
				for (int i = 0; i < len; ++i)
					ss << std::setw(2) << std::setfill('0') << (int)data[i];
				return ss.str();
			}

		};
	}
}
