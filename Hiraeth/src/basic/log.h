#pragma once
#include "spdlog/spdlog.h"

namespace hiraeth
{
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& getLogger() { return m_Logger; }
	private:
		static std::shared_ptr<spdlog::logger> m_Logger;
		//log();
		//~log();
	};
}

#define HH_LOG(...) ::hiraeth::Log::getLogger()->info(__VA_ARGS__)

	//for (int i = 0; i < sizeof(monster_damage); i++)
	//{
	//	printf("%02X ", (message)[i]);
	//}
	//printf("\n");
