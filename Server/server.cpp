#include "server.h"
#include "srl/skill_data.h"
#include "srl/equip_item_data.h"

//#include <windows.h>


namespace hiraeth {
	namespace network {
		/*
		 * This implementation uses a 60 fps receive and process loop with run-time (non_sleep)
		 * prints.
		 */



		void Server::main_60fps_loop()
		{
			m_Socket.SetUnblocking();
			float dt = 0;
			int fps_count = 0;
			ATimer stimer{ 1.0f };
			std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
			while (true)
			{
				std::thread timer{ [] {std::this_thread::sleep_for(std::chrono::milliseconds(16)); } };
				while (true)
				{
					Address sender;
					const int bytes_read =
						m_Socket.Receive(sender, m_Buffer, sizeof(m_Buffer));
					if (bytes_read > 0)
						switchData(sender);
					else
						break;
				}
				++fps_count;
				if (stimer.hasExpired())
				{
					printf("hey lo? %f and fps is %d\n", dt, fps_count);
					//std::cout << "hey lo? " << dt << " and fps is " << fps_count << "\n";
					stimer.reSet(1.0f);
					dt = 0;
					fps_count = 0;
				}
				dt += std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start).count();
				timer.join();
				start = std::chrono::high_resolution_clock::now();
			}
		}

		/*
		 * This implementation uses one loop to receive and immidietly process data received,
		 * and enables a send_after functionality sending messages using async sending.
		 */
		void Server::main_block_receive_and_async_send()
		{
			while (true)
			{
				std::future<void> timed_message =
					std::async(std::launch::async,
						[this] {std::this_thread::sleep_for(std::chrono::milliseconds(2000));
				char data[1];
				data[0] = MSG_INR_UPDATE_MOB_CMD;
				Address address{ 127,0,0,1, 8888 };
				m_Socket.Send(address, data, 1);
				//delete[] data;
				});
				while (true)
				{
					Address sender;
					const int bytes_read =
						m_Socket.Receive(sender, m_Buffer, sizeof(m_Buffer));
					if (bytes_read > 0)
						switchData(sender);
					else
						break;
				}
			}
		}

		/*
		 * This function uses 1 thread for receiving data, 1 thread for processing data, and
		 * more threads for every send_after type messages
		 */
		void Server::main_block_threaded_queue()
		{
			std::thread readThread{ [this] {this->dataReader(); } };

			createMessageThread(MSG_INR_UPDATE_MOB_CMD, 3000);

			while (true)
			{
				std::unique_lock<std::mutex> lock{ m_Mutex };
				m_Cv.wait(lock, [this] {return !m_DataQueue.empty(); });

				while (true)
				{
					if (!lock.owns_lock())
						lock.lock();
					auto[data, bytes_read, sender] = m_DataQueue.front();
					m_DataQueue.pop();
					lock.unlock();

					memcpy(m_Buffer, data, bytes_read);
					delete[] data;

					switchData(sender);
					if (m_DataQueue.empty())
						break;
				}
			}
		}
		//void Server::dataReaderCv()
		//{
		//	while (true)
		//	{
		//		Address sender;
		//		char * buffer = new char[256];
		//		const int bytes_read =
		//			m_Socket.Receive(sender, buffer, 256);
		//		{
		//			std::lock_guard<std::mutex> lock{ m_Mutex };
		//			m_DataQueue.push(QueueData{ buffer, bytes_read, sender });
		//		}
		//		m_Cv.notify_one();
		//	}
		//}


		/*
		 * This function uses 1 thread for receiving data, 1 thread for processing data, and
		 * more threads for every send_after type messages
		 */
		void Server::main_60fps_threaded_queue()
		{

			std::thread readThread{ [this] {this->dataReader(); } };
			//createMessageThread(MSG_INR_UPDATE_MOB_CMD, 5000);
			createMessageThread(MSG_INR_MOB_UPDATE, 5000);
			createMessageThread(MSG_INR_ROUTINE_UPDATE, 1000);
			//createMessageThread(MSG_INR_FIND_MOB_POS, 5000);
			//createMessageThread(MSG_INR_MOB_UPDATE, 1000);

			while (true)
			{
				std::future<void> timer =
					std::async(std::launch::async,
						[] {std::this_thread::sleep_for(std::chrono::milliseconds(16)); });
				std::unique_lock<std::mutex> lock{ m_Mutex };
				m_Cv.wait(lock, [this] {return !m_DataQueue.empty(); });
				while (true)
				{
					if (!lock.owns_lock())
						lock.lock();
					auto[data, bytes_read, sender] = m_DataQueue.front();
					m_DataQueue.pop();
					lock.unlock();

					memcpy(m_Buffer, data, bytes_read);
					m_RecvSize = bytes_read;
					delete[] data;
					if (bytes_read > 0)
					{
						switchData(sender);
					}
					else
						break;

					if (m_DataQueue.empty())
						break;
				}
				timer.wait();
			}
		}
		void Server::dataReader()
		{
			while (true)
			{
				Address sender;
				char * buffer = new char[256];
				const int bytes_read =
					m_Socket.Receive(sender, buffer, 256);
				{
					std::lock_guard<std::mutex> lock{ m_Mutex };
					m_DataQueue.push(QueueData{ buffer, bytes_read, sender });
				}
				m_Cv.notify_one();
			}
		}

		void Server::addMessageAfterT(int time, char * buffer, int size)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(time));
			{
				std::lock_guard<std::mutex> lock{ m_Mutex };
				m_DataQueue.push(QueueData{ buffer, size, {} });
			}
		}
		void Server::createMessageThread(char MessageType, int milliseconds)
		{
			char * buffer = new char[256];
			buffer[0] = MessageType;
			std::thread message_thread{ &Server::addMessageAfterT, this, milliseconds, buffer, 1 };
			message_thread.detach();
		}

		void Server::switchData(Address sender)
		{
			if (m_DistTable.find(m_Buffer[0]) != m_DistTable.end())
			{
				m_DistTable[m_Buffer[0]](sender);
				//(this->*m_DistTable2[m_Buffer[0]])(sender);
			}
		}

		unsigned int Server::sendConnectionResponse(Address sender)
		{
			unsigned int map_id{ 0 };
			auto name = dsrl_dynamic_type<std::string>(m_Buffer + 1);
			auto char_id = m_DbClient->getIdByAccountName(name);
			if (m_NumConnectedClients > 0)
				char_id = 2;
			//const auto new_char_id = findFreeClientIndex();
			const auto new_char_id = char_id;
			//sendNewPlayerInMap(new_char_id);
			m_NumConnectedClients++;
			m_ClientAddress[new_char_id] = sender;
			m_ClientsIds.push_back(new_char_id);
			//m_ClientConnected[free_client_index] = true;
			//m_DbClient->setByteArray(1, "skills_alloc", std::vector<SkillAlloc> {{666,20}, 
			//	{0, 1}, {1,1}, {2,1}, {5,1},
			//	{6,1}, {7,1}, {8,1}, {9,1}});
			m_DbClient->setByteArray(m_NumConnectedClients, "player_state", decltype(PlayerData::player_hold_state) { });
			m_DbClient->setByteArray(m_NumConnectedClients, "quests_in_prog", decltype(PlayerData::quests_in_progress) { });
			m_DbClient->setByteArray(m_NumConnectedClients, "quests_done", decltype(PlayerData::quests_done) { });
			//m_DbClient->setByteArray(1, "inv_equip", decltype(PlayerData::inv_equip) { {1, 1}});
			//m_DbClient->setByteArray(char_id, "inv_equip", decltype(PlayerData::inv_equip) {
			//	{0, SRL::EquipDbStruct{ 1, {{SRL::AttackPower, 10}, {SRL::StrInc, 5}} }},
			//	{ 1, SRL::EquipDbStruct{ 1, {{SRL::AttackPower, 10}, {SRL::StrInc, 5}} } },
			//	{ 2, SRL::EquipDbStruct{ 2, {{SRL::LukInc, 7}, {SRL::IntInc, 7}} } },
			//	{ 3, SRL::EquipDbStruct{ 3, {{SRL::AttackPower, 11}, {SRL::StrInc, 3}} } },
			//	{ 4, SRL::EquipDbStruct{ 4, {{SRL::AttackPower, 12}, {SRL::StrInc, 7}} } },
			//	{ 5, SRL::EquipDbStruct{ 1, {{SRL::AttackPower, 10}, {SRL::StrInc, 5}} } },
			//	{ 6, SRL::EquipDbStruct{ 4, {{SRL::AttackPower, 10}, {SRL::StrInc, 5}} } },
			//	{ 7, SRL::EquipDbStruct{ 3, {{SRL::AttackPower, 10}, {SRL::StrInc, 5}} } },
			//	{ 8, SRL::EquipDbStruct{ 4, {{SRL::AttackPower, 10}, {SRL::StrInc, 5}} } },
			//	{ 9, SRL::EquipDbStruct{ 2, {{SRL::AttackPower, 10}, {SRL::StrInc, 5}} } },
			//	{ 10, SRL::EquipDbStruct{ 1, {{SRL::AttackPower, 10}, {SRL::StrInc, 5}} } },
			//});
			//m_DbClient->setByteArray(1, "inv_use", decltype(PlayerData::inv_use) { });
			//m_DbClient->setByteArray(1, "inv_setup", decltype(PlayerData::inv_setup) { });
			//m_DbClient->setByteArray(1, "inv_etc", decltype(PlayerData::inv_etc) { });
			//m_DbClient->setByteArray(1, "inv_cash", decltype(PlayerData::inv_cash) { });
			//m_DbClient->setByteArray(char_id, "equips_char", decltype(PlayerData::equips_char) { });
			//m_DbClient->setByteArray(1, "item_reqs", 
			//	std::map<SRL::EquipReqEnum, int> { {SRL::eReqLvl, 5}, {SRL::eReqStr, 3}}, "equips");
			//m_DbClient->setByteArray(1, "properties", 
			//	std::map<SRL::EquipItemDataType, int> { {SRL::StrInc, 5}, {SRL::IntInc, 3}}, "equips");
			const auto player_data = m_DbClient->getPlayerDataById(char_id);
			m_PlayersState[char_id] = player_data.player_hold_state;
			m_PlayersStats[char_id] = player_data.player_stats;
			m_PlayersMsgs[char_id] = PlayerMsgState{};
			m_PlayerToMapId[char_id] = map_id;
			m_MapHolder[m_PlayerToMapId[char_id]].players_equips[char_id] = player_data.equips_char;

						
			//auto player_data = m_DbClient->getPlayerDataById(char_id);

			//auto player_data = PlayerData{"shd", 10, 1, 0, 300, 300, {}, {}};
			//player_data.stats_alloc = std::vector<unsigned int>{ 1,1,1 };
			//player_data.skills_alloc = {0,0,0,1,1,1};
			//m_DbClient->setStatsAlloc(new_char_id, player_data.stats_alloc);
			//player_data.stat_allocation = m_DbClient->getStatsAlloc(new_char_id);
			auto equip_items = player_data.inv_equip;
			ConnectionEstablishMsg msg{new_char_id, 
				player_data};
			msg.player_data.inv_equip = {};
			auto[data, size] = srl_dynamic_type(msg);
			const auto buffer_size = construct_server_packet_with_buffer(m_Buffer,
				MSG_STC_ESTABLISH_CONNECTION, *data, size);
			printf("registered new address : %s , and port is : %d , and id is %d\n",
				sender.GetAddressString().c_str(), sender.GetPort(), new_char_id);
			m_Socket.Send(sender, m_Buffer, buffer_size);

			m_MapHolder[map_id].sendNewPlayerInMap(new_char_id);
			for (auto [item_loc, item] : equip_items)
			{
				auto [data, size] = srl_dynamic_type(AddEquipItemMsg{ item_loc, 0, item});
					m_Size = construct_server_packet_with_buffer(m_Buffer, MSG_STC_ADD_EQUIP_ITEM,
						*data, size);
					m_Socket.Send(sender, m_Buffer, m_Size);
			}
			//sendDropItem(ItemDropMsg{ 20, 1, USE_ITEM, maths::vec2{ 55, 15 } });
			sendDroppedItems(map_id, sender);
			return map_id;
		}

		void Server::closeConnection(BufferType* buffer)
		{
			const auto id = dsrl_type<unsigned int>(buffer + 1);
			printf("unregistered id is %d\n", id);
			m_NumConnectedClients--;
			m_ClientAddress[id] = Address{};
			//m_ClientConnected[id] = false;
			m_ClientsIds.erase(std::remove(m_ClientsIds.begin(), m_ClientsIds.end(), id), m_ClientsIds.end());
		}

		void Server::receiveLocation(BufferType* buffer)
		{
			const auto player_id = dsrl_type<unsigned int>(buffer + 1);
			const auto char_state = dsrl_type<PlayerStateUpdateMsg>(buffer + 5);
			//m_ClientsState[player_id] = char_state;
			m_MapHolder[m_PlayerToMapId[player_id]].players_state[player_id] = char_state;
		}

	}
}
