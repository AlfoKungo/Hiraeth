#include "server.h"

#include <windows.h>
#include <io.h>
#include <fcntl.h>


namespace hiraeth {
	namespace network {
		void Server::main_function()
		{
			if (!m_Socket.Open(PORT))
			{
				printf("failed to create socket!\n");
				return;
			}

			m_Socket.SetUnblocking();
			float dt = 0;
			int fps_count = 0;
			ATimer stimer{ 1.0f };
			std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
			while (true)
			{
				std::future<void> timer =
					std::async(std::launch::async,
						[] {std::this_thread::sleep_for(std::chrono::milliseconds(16)); });
				while (true)
				{
					Address sender;
					const int bytes_read =
						m_Socket.Receive(sender, m_Buffer, sizeof(m_Buffer));
					if (bytes_read > 0)
						switch (m_Buffer[0])
						{
						case MSG_CTS_OPEN_CONNECTION:
							sendConnectionResponse(sender);
							break;
						case MSG_CTS_CLOSE_CONNECTION:
							closeConnection(m_Buffer);
							break;
						case MSG_CTS_LOCATION_UPDATE:
							receiveLocation(m_Buffer);
							sendUpdateLocationToAll(sender);
							break;
						case MSG_CTS_KA:
							//sendKeepAliveAnswer(sender);
							sendUpdateLocationToAll(sender);
							break;
						default:
							break;
						}
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
				timer.wait();
				start = std::chrono::high_resolution_clock::now();
			}
		}

		void Server::altMain()
		{
			if (!m_Socket.Open(PORT))
			{
				printf("failed to create socket!\n");
				return;
			}

			while (true)
			{
				//std::future<void> timed_message =
				//	std::async(std::launch::async,
				//		[this] {std::this_thread::sleep_for(std::chrono::milliseconds(2000));
				//			char data[1];
				//			data[0] = 0x22;
				//			Address address{127,0,0,1, 8888};
				//			m_Socket.Send(address, data, 1);
				//			//int fd = _open_osfhandle((intptr_t)m_Socket.handle, 0);
				//			//FILE *f_recv = _fdopen(fd, "a+");
				//			//fwrite(data, 1, 2, f_recv);
				//			//_write(m_Socket.handle, data, 1);
				//			//send(m_Socket.handle, data, 1, 0);
				//			//DWORD bytes_written;
				//			//WriteFile(&m_Socket.handle, data, 1, &bytes_written, 0);
				//			//delete[] data;
				//		});
				while (true)
				{
					Address sender;
					const int bytes_read =
						m_Socket.Receive(sender, m_Buffer, sizeof(m_Buffer));
					if (bytes_read > 0)
						switch (m_Buffer[0])
						{
						case MSG_CTS_OPEN_CONNECTION:
							sendConnectionResponse(sender);
							break;
						case MSG_CTS_CLOSE_CONNECTION:
							closeConnection(m_Buffer);
							break;
						case MSG_CTS_LOCATION_UPDATE:
							receiveLocation(m_Buffer);
							sendUpdateLocationToAll(sender);
							break;
						case MSG_CTS_KA:
							//sendKeepAliveAnswer(sender);
							sendUpdateLocationToAll(sender);
							break;
						case 0x22:
							//sendKeepAliveAnswer(sender);
							sendUpdateLocationToAll(sender);
							break;
						default:
							break;
						}
					else
						break;
				}
			}
		}
		void Server::altMain2()
		{
			if (!m_Socket.Open(PORT))
			{
				printf("failed to create socket!\n");
				return;
			}
			std::thread readThread{ [this] {this->dataReader(); } };
			//std::thread sendThread{ [this]
			//	{
			//	char * buffer = new char[256];
			//	char data[] = "h";
			//	const auto buffer_size = construct_server_packet(buffer,
			//												 MSG_INR_SUMMON_MONSTER, data, 1);
			//	this->addMessageIn(10000, buffer, buffer_size);
			//} };

			//std::unique_lock<std::mutex> lock{ m_Mutex };
			//m_Cv.wait(lock, [this] {return m_DataQueue.empty(); });
			//m_Cv.wait(lock);
			while (true)
			{
				std::future<void> timer =
					std::async(std::launch::async,
						[] {std::this_thread::sleep_for(std::chrono::milliseconds(16)); });
				while (!m_DataQueue.empty())
				{
					QueueData queue_data;
					{
						std::lock_guard<std::mutex> lock{ m_Mutex };
						//m_Mutex.lock();
						queue_data = m_DataQueue.front();
						m_DataQueue.pop();
					}
					auto[data, bytes_read, sender] = queue_data;

					memcpy(m_Buffer, data, bytes_read);
					delete[] data;
					if (bytes_read > 0)
					{
						switch (m_Buffer[0])
							//switch (data[0])
						{
						case MSG_CTS_OPEN_CONNECTION:
							sendConnectionResponse(sender);
							break;
						case MSG_CTS_CLOSE_CONNECTION:
							closeConnection(m_Buffer);
							break;
						case MSG_CTS_LOCATION_UPDATE:
							receiveLocation(m_Buffer);
							sendUpdateLocationToAll(sender);
							break;
						case MSG_CTS_KA:
							//sendKeepAliveAnswer(sender);
							sendUpdateLocationToAll(sender);
							break;
						case MSG_INR_SUMMON_MONSTER:
							//sendKeepAliveAnswer(sender);
							sendUpdateLocationToAll(sender);
							break;
						default:
							break;
						}
					}
					else
						break;

					//m_Mutex.unlock();
					timer.wait();

				}
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
				//m_Mutex.lock();
				{
					std::lock_guard<std::mutex> lock{ m_Mutex };
					m_DataQueue.push(QueueData{ buffer, bytes_read, sender });
				}
				//m_Mutex.unlock();
				//m_Cv.notify_one();
			}
		}

		void Server::addMessageIn(int time, char * buffer, int size)
		{

			{std::this_thread::sleep_for(std::chrono::milliseconds(time)); };
			//m_Mutex.lock();
			{
				std::lock_guard<std::mutex> lock{ m_Mutex };
				m_DataQueue.push(QueueData{ buffer, size, {} });
			}
			//m_Mutex.unlock();
		}

		void Server::sendConnectionResponse(Address sender)
		{
			const auto free_client_index = FindFreeClientIndex();
			sendNewPlayerInMap(free_client_index);
			m_numConnectedClients++;
			m_ClientAddress[free_client_index] = sender;
			m_ClientConnected[free_client_index] = true;
			char data[4];
			unsigned int id = free_client_index;
			memcpy(data, &id, sizeof(unsigned int));
			printf("registered new address : %s , and port is : %d , and id is %d\n",
			       sender.GetAddressString().c_str(), sender.GetPort(), id);
			m_Socket.Send(sender, data, sizeof(data));
		}

		void Server::sendNewPlayerInMap(unsigned int new_char_index)
		{
			for (const auto& client : m_ClientAddress)
			{
				if (client.GetAddress() != 0)
				{
					char* message = new char[4];
					memcpy(message, &new_char_index, sizeof(new_char_index));
					auto size = construct_server_packet(m_Buffer, MSG_STC_ADD_PLAYER, message, 4);
					m_Socket.Send(client, m_Buffer, size);
				}
			}
		}

		void Server::closeConnection(char* buffer)
		{
			unsigned int id;
			memcpy(&id, buffer + 1, sizeof(unsigned int));
			printf("unregistered id is %d\n", id);
			m_numConnectedClients--;
			m_ClientAddress[id] = Address{};
			m_ClientConnected[id] = false;
		}

		void Server::receiveLocation(char* buffer)
		{
			//unsigned int id;
			//dsrl_packet_data(id, buffer + 1);
			//maths::vec2 char_pos;
			//dsrl_packet_data(char_pos, buffer + 5);
			const auto id = dsrl_packet_data<unsigned int>(buffer + 1);
			const auto char_state = dsrl_packet_data<PlayerStateUpdate>(buffer + 5);
			m_ClientsState[id] = char_state;
		}

		void Server::sendUpdateLocationToAll(Address sender)
		{
			const auto id = dsrl_packet_data<unsigned int>(m_Buffer + 1);
			//auto [data, size] = srl_packet_data(m_ClientsState);
			RegularMapUpdate map_update_data{m_ClientsState};
			//map_update_data.m_PlayersLocation.erase(id); // erase this line to research delay
			auto [data, size] = srl_packet_data(map_update_data);
			const auto buffer_size = construct_server_packet(m_Buffer,
			                                                 MSG_STC_PLAYERS_LOCATIONS, data, size);
			m_Socket.Send(sender, m_Buffer, buffer_size);
			delete[] data;
			//char * data = new char[sizeof(maths::vec2) * m_ClientsIds.size()];
			//char data[sizeof(maths::vec2) * m_ClientsIds.size()];
			//for (const auto& client_id : m_ClientsIds)
			//{
			//	
			//}

			//std::stringstream os{ std::ios::binary | std::ios::out };
			//{
			//	cereal::BinaryOutputArchive ar(os);
			//	ar(m_ClientsPos);
			//} // the binary archives will flush their output 
			//std::string data_str = os.str();
			//auto data = new char[data_str.size() + 1];
			//data[0] = char(data_str.size());
			//memcpy(data + 1, data_str.c_str(), data_str.size());
			//m_Socket.Send(sender, data, data_str.size() + 1);
		}
	}
}