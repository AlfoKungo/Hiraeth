#pragma once

#include <cereal/archives/binary.hpp>
#include <cereal/cereal.hpp>

inline const char * DF_QUEST = "quest";
inline const char * DF_MAP = "map";
inline const char * DF_MONSTER = "monster";
inline const char * DF_NPC = "npc";
inline const char * DF_EQUIP = "equip";

namespace SRL {
	template <class Rtype>
	static Rtype deserial(const char* datafile_name, unsigned int address)
	{
		std::string path = "serialized/";
		path += datafile_name;
		path += ".data";
			//std::ifstream file(datafile_name, std::ios::in | std::ios::binary);
		std::ifstream file(path, std::ios::in | std::ios::binary);
		cereal::BinaryInputArchive iarchive(file);
		Rtype rType{};
		file.seekg((address) * sizeof(int));
		int start_of_data;
		iarchive(start_of_data);
		file.seekg(start_of_data);
		iarchive(rType);
		return rType;
	}
}
				//const char *paramValues[1];
				//paramValues[0] = "2";
				//PGresult   *res;

				//res = PQexecParams(conn,
				//	"SELECT * FROM players WHERE id = $1",
				//	1,       /* one param */
				//	NULL,    /* let the backend deduce param type */
				//	paramValues,
				//	NULL,    /* don't need param lengths since text */
				//	NULL,    /* default to all text params */
				//	1);      /* ask for binary results */

				//if (PQresultStatus(res) != PGRES_TUPLES_OK)
				//{
				//	fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
				//	PQclear(res);
				//}

				//char *name;
				////int *level, *exp, *job;
				//int loc = PQfnumber(res, "name");
				///* Get the field values (we ignore possibility they are null!) */
				//name = PQgetvalue(res, 0, loc);
				//int         blen;
				//blen = PQgetlength(res, 0, loc);
				//printf(" b = (%d bytes) ", blen);
				//std::cout <<  name;
				//for (int j = 0; j < blen; j++)
				//	printf("\\%03o", name[j]);

				//PQclear(res);
