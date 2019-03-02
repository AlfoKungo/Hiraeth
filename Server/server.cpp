#include "server.h"

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
					m_Size = bytes_read;
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
			//switch (m_Buffer[0])
			//{
			//case MSG_CTS_OPEN_CONNECTION:
			//	sendConnectionResponse(sender);
			//	m_MobManager.recalculateAllMobs();
			//	sendMobsData(sender);
			//	break;
			//case MSG_CTS_CLOSE_CONNECTION:
			//	closeConnection(m_Buffer);
			//	break;
			//case MSG_CTS_LOCATION_UPDATE:
			//	receiveLocation(m_Buffer);
			//	sendUpdateLocationToAll(sender);
			//	break;
			//case MSG_CTS_KA:
			//	sendUpdateLocationToAll(sender);
			//	break;
			//case MSG_CTS_HIT_MOB:
			//	//sendUpdateLocationToAll(sender);
			//	break;
			//case char(MSG_INR_UPDATE_MOB_CMD) :
			//	//m_MobManager.setNewMoveCommand(0, MobMoveCommand{ Left, ATimer{15.0f}, 15 });
			//	//sendMobsUpdate(0, MobMoveCommand{ Left, ATimer{5.0f}, 5 });
			//	//sendMobsUpdate(1, MobMoveCommand{ Right, ATimer{5.0f}, 5 });
			//	//createMessageThread(MSG_INR_MOB_UPDATE, 1000);
			//	break;
			//case char(MSG_INR_MOB_HIT) :
			//	sendMobGotHit(0, Left);
			//	//m_MobManager.calculateLocationNow(0);
			//	break;
			//	//case char(MSG_INR_FIND_MOB_POS):
			//	//	sendMobsUpdate(0, MobMoveCommand{Stand, ATimer{15.0f}, 15});
			//	//	//m_MobManager.calculateLocationNow(0);
			//	//	break;
			//case char(MSG_INR_MOB_UPDATE) :
			//	updateMobManager();
			//	createMessageThread(MSG_INR_MOB_UPDATE, 1000);
			//	break;
			//default:
			//	break;
			//}
		}

		void Server::sendConnectionResponse(Address sender)
		{
			const auto new_char_id = findFreeClientIndex();
			sendNewPlayerInMap(new_char_id);
			m_numConnectedClients++;
			m_ClientAddress[new_char_id] = sender;
			m_ClientsIds.push_back(new_char_id);
			//m_ClientConnected[free_client_index] = true;

			ConnectionEstablishMsg msg{new_char_id, 
				{69, "Kanye", 1, {0,0,0,0}, {0,0,0,1,1,1}}};
			auto[data, size] = srl_dynamic_type(msg);
			const auto buffer_size = construct_server_packet_with_buffer(m_Buffer,
				MSG_STC_ESTABLISH_CONNECTION, *data, size);
			printf("registered new address : %s , and port is : %d , and id is %d\n",
				sender.GetAddressString().c_str(), sender.GetPort(), new_char_id);
			m_Socket.Send(sender, m_Buffer, buffer_size);
		}

		void Server::sendNewPlayerInMap(unsigned int new_char_index)
		{
			const auto size = construct_server_packet(m_Buffer, MSG_STC_ADD_PLAYER, new_char_index);
			sendDataToAllClients(size);
		}

		void Server::closeConnection(BufferType* buffer)
		{
			const auto id = dsrl_type<unsigned int>(buffer + 1);
			printf("unregistered id is %d\n", id);
			m_numConnectedClients--;
			m_ClientAddress[id] = Address{};
			//m_ClientConnected[id] = false;
			m_ClientsIds.erase(std::remove(m_ClientsIds.begin(), m_ClientsIds.end(), id), m_ClientsIds.end());
		}

		void Server::receiveLocation(BufferType* buffer)
		{
			const auto id = dsrl_type<unsigned int>(buffer + 1);
			const auto char_state = dsrl_type<PlayerStateUpdateMsg>(buffer + 5);
			m_ClientsState[id] = char_state;
		}

		void Server::sendUpdateLocationToAll(Address sender)
		{
			RegularMapUpdate map_update_data{ m_ClientsState };
			const auto client_id = dsrl_type<unsigned int>(m_Buffer + 1);
			map_update_data.m_PlayersLocation.erase(client_id); // erase this line to research delay
			auto[data, size] = srl_dynamic_type(map_update_data);
			const auto buffer_size = construct_server_packet_with_buffer(m_Buffer,
				MSG_STC_PLAYERS_LOCATIONS, *data, size);
			m_Socket.Send(sender, m_Buffer, buffer_size);
		}

		void Server::sendMobsData(Address sender)
		{
			auto[data, size] = srl_dynamic_type(m_MobManager.m_Monsters);
			const auto buffer_size = construct_server_packet_with_buffer(m_Buffer,
				MSG_STC_MOB_DATA, *data, size);
			m_Socket.Send(sender, m_Buffer, buffer_size);
		}

		void Server::sendMobsUpdate(unsigned int mob_id, MobMoveCommand mmc)
		{
			printf("mob %d is commanded to %d for %f seconds\n", mob_id, mmc.dir, mmc.duration);
			MonsterStateUpdate& state_data = m_MobManager.m_Monsters[mob_id];
			m_MobManager.setNewMoveCommand(mob_id, mmc);
			const auto size = construct_server_packet(m_Buffer, MSG_STC_MOB_UPDATE, mob_id, state_data);
			sendDataToAllClients(size);
		}

		void Server::updateMobManager()
		{
			auto mob_ids = m_MobManager.update();
			for (const auto& id : mob_ids)
				sendMobsUpdate(id, m_MobManager.m_MoveCmds[id]);
		}
	}
}
