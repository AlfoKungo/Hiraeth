#include "server.h"

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
			while (true)
			{
				//std::packaged_task<void> = 
				//std::future<void> timed_message =
				//	std::async(std::launch::async,
				//		[this] {std::this_thread::sleep_for(std::chrono::milliseconds(16));
				//			Address ad{};
				//			char data[] = "hello";
				//			m_Socket.Send(ad, data, 5);
				//		});
				std::future<void> timer =
					std::async(std::launch::async,
						[] {std::this_thread::sleep_for(std::chrono::milliseconds(16)); });
				//ATimer atimer{0.016f};
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
				std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
				timer.wait();
				dt += std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start).count();
				++fps_count;
				if (stimer.hasExpired())
				{
					printf("hey lo? %f and fps is %d\n", dt, fps_count);
					//std::cout << "hey lo? " << dt << " and fps is " << fps_count << "\n";
					stimer.reSet(1.0f);
					dt = 0;
					fps_count = 0;
				}
			}
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
			map_update_data.m_PlayersLocation.erase(id); // erase this line to research delay
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