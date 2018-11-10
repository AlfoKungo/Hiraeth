#pragma once
#include "protocol.h"

namespace hiraeth {
	namespace network {

		template<class T>
		//std::tuple<char *, int> srl_packet_data(const T& srl_data)
		std::tuple<std::unique_ptr<char*>, int> srl_packet_data(const T& srl_data)
		{
			std::stringstream os{ std::ios::binary | std::ios::out };
			{
				cereal::BinaryOutputArchive ar(os);
				ar(srl_data);
			} // the binary archives will flush their output 
			std::string data_str = os.str();
			auto data = new char[data_str.size() + 1];
			data[0] = char(data_str.size());
			memcpy(data + 1, data_str.c_str(), data_str.size());
			return { std::move(std::make_unique<char*>(data)), data_str.size() + 1 };
			//return { data, data_str.size() + 1};
		}

		template<typename T>
		int srl_packet_data(BufferType * buffer, T&& var)
		{
			memcpy(buffer, &var, sizeof(var));
			return sizeof(var);
		}
		template<typename T, typename ... Ts>
		int srl_packet_data(BufferType * buffer, T&& var,  Ts&&... vars)
		{
			memcpy(buffer, &var, sizeof(var));
			return sizeof(var) + srl_packet_data(buffer + sizeof(var), std::forward<Ts...>(vars...));
		}

		template<typename T>
		void dsrl_packet_data(BufferType * buffer, T& var)
		{
			memcpy(&var, buffer, sizeof(var));
		}
		template<typename T, typename ... Ts>
		void dsrl_packet_data(BufferType * buffer, T& var,  Ts&... vars)
		{
			memcpy(&var, buffer,  sizeof(var));
			dsrl_packet_data(buffer + sizeof(var), vars...);
		}

		/*
		 * a function for deserializing simple data structs
		 */
		template<class T>
		T dsrl_packet_data(BufferType * buffer)
		{
			T dsrl_data;
			memcpy(&dsrl_data, buffer, sizeof(dsrl_data));
			return std::move(dsrl_data);
		}

		template<class T>
		void dsrl_packet_data_nr(T& dsrl_data, char * buffer)
		{
			memcpy(&dsrl_data, buffer, sizeof(dsrl_data));
		}

		/*
		 * void dsrl_d(ynamic)t(ype)_packet_data(T& dsrl_data, char * buffer)
		 * 
		 * a function for deserializing dynamic data types. requires the buffer
		 * to contain the size of the data in the first slot.
		 */
		template<class T>
		void dsrl_packet_data_dynamic_type(T& dsrl_data, BufferType * buffer)
		{
			const std::string tmp_str{ reinterpret_cast<char * >(buffer) + 1, static_cast<unsigned int>(buffer[0]) };
			std::stringstream is(tmp_str, std::ios::binary | std::ios::in);
			{
				cereal::BinaryInputArchive ar(is);
				ar(dsrl_data);
			}
		}
	}
}
