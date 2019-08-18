#pragma once
#include "protocol.h"

namespace hiraeth {
	namespace network {

		template<typename T>
		int srl_types(BufferType * buffer, T&& var)
		{
			memcpy(buffer, &var, sizeof(var));
			return sizeof(var);
		}
		//template<typename T>
		//typename std::enable_if<std::is_pod<T>::value>::type
		// srl_types(BufferType * buffer, T&& var)
		//{
		//	memcpy(buffer, &var, sizeof(var));
		//	return sizeof(var);
		//}
		template<typename T, typename ... Ts>
		int srl_types(BufferType * buffer, T&& var,  Ts&&... vars)
		{
			memcpy(buffer, &var, sizeof(var));
			//return sizeof(var) + srl_types(buffer + sizeof(var), std::forward<Ts...>(vars...));
			return sizeof(var) + srl_types(buffer + sizeof(var), std::forward<Ts>(vars)...);
		}
		//template<typename T, typename ... Ts>
		//int srl_types(BufferType * buffer, T&& var,  Ts&&... vars)
		//{
		//	memcpy(buffer, &var, sizeof(var));
		//	//return sizeof(var) + srl_types(buffer + sizeof(var), std::forward<Ts...>(vars...));
		//	return sizeof(var) + srl_types(buffer + sizeof(var), std::forward<Ts>(vars)...);
		//}

		template<typename T>
		void dsrl_type(BufferType * buffer, T& var)
		{
			memcpy(&var, buffer, sizeof(var));
		}
		template<typename T, typename ... Ts>
		void dsrl_type(BufferType * buffer, T& var,  Ts&... vars)
		{
			memcpy(&var, buffer,  sizeof(var));
			dsrl_type(buffer + sizeof(var), vars...);
		}

		/*
		 * a function for deserializing simple data structs
		 */
		template<class T>
		T dsrl_type(BufferType * buffer)
		{
			T dsrl_data;
			memcpy(&dsrl_data, buffer, sizeof(dsrl_data));
			return std::move(dsrl_data);
		}

		template<typename T>
		std::tuple<T> dsrl_types(BufferType * buffer)
		{
			T dsrl_data;
			memcpy(&dsrl_data, buffer, sizeof(dsrl_data));
			return std::make_tuple(dsrl_data);
		}
		template<typename T1, typename T2, typename ... Ts>
		std::tuple<T1, T2, Ts...> dsrl_types(BufferType * buffer)
		{
			T1 dsrl_data;
			memcpy(&dsrl_data, buffer, sizeof(dsrl_data));
			return std::tuple_cat(std::make_tuple(dsrl_data), dsrl_types<T2, Ts...>(buffer + sizeof(T1)));
		}
		

		template<class T>
		//std::tuple<char *, int> srl_packet_data(const T& srl_data)
		std::tuple<std::unique_ptr<BufferType*>, int> srl_dynamic_type(const T& srl_data)
		{
			std::stringstream os{ std::ios::binary | std::ios::out };
			{
				cereal::BinaryOutputArchive ar(os);
				ar(srl_data);
			} // the binary archives will flush their output 
			std::string data_str = os.str();
			auto data = new BufferType[data_str.size() + 1];
			data[0] = char(data_str.size());
			data[1] = char(data_str.size() >> 8) & 0xff;
			//memcpy(data + 1, data_str.c_str(), data_str.size());
			//return { std::make_unique<BufferType*>(data), data_str.size() + 1 };
			memcpy(data + 2, data_str.c_str(), data_str.size());
			return { std::make_unique<BufferType*>(data), data_str.size() + 2 };
		}
		/*
		 * void dsrl_d(ynamic)t(ype)_packet_data(T& dsrl_data, char * buffer)
		 * 
		 * a function for deserializing dynamic data types. requires the buffer
		 * to contain the size of the data in the first slot.
		 */
		template<class T>
		void dsrl_dynamic_type(T& dsrl_data, const BufferType * buffer)
		{
			//const std::string tmp_str{ reinterpret_cast<const BufferType * >(buffer) + 1, static_cast<unsigned int>(buffer[0]) };
			const unsigned int size = static_cast<unsigned char>(buffer[0]) + int(static_cast<unsigned char>(buffer[1])) * 256;
			//const std::string tmp_str{ (buffer) + 1, size };
			const std::string tmp_str{ (buffer) + 2, size };
			std::stringstream is(tmp_str, std::ios::binary | std::ios::in);
			{
				cereal::BinaryInputArchive ar(is);
				ar(dsrl_data);
			}
		}
		template<class T>
		T dsrl_dynamic_type(const BufferType * buffer)
		{
			T dsrl_data;
			dsrl_dynamic_type(dsrl_data, buffer);
			return dsrl_data;
		}
	}
}



		//class BufferHolder
		//{
		//	BufferType * m_Buffer;
		//public:
		//	BufferHolder(BufferType * buffer)
		//		: m_Buffer(buffer)
		//	{
		//	}
		//	template<class T>
		//	T dsrl_types()
		//	{
		//		T dsrl_data;
		//		memcpy(&dsrl_data, m_Buffer, sizeof(dsrl_data));
		//		m_Buffer += sizeof(T);
		//		return std::move(dsrl_data);
		//	}
		//	template<class T>
		//	void copy_type(T & val)
		//	{
		//		memcpy(&val, m_Buffer, sizeof(val));
		//		m_Buffer += sizeof(T);
		//	}
		//};

		//template<class T>
		//T& Dsrl_types(BufferHolder& buffer_holder)
		//{
		//	static T dsrl_data;
		//	buffer_holder.copy_type(dsrl_data);
		//	return dsrl_data;
		//}
		//template<typename ... T>
		//std::tuple<T...> dsrl_typesss(BufferType * buffer)
		//{
		//	BufferHolder buffer_holder(buffer);
		//	return std::tuple_cat(Dsrl_types<T>(buffer_holder)...);
		//}
