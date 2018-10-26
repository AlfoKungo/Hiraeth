#include "address.h"

namespace hiraeth {
	namespace network {
		Address::Address()
			: address{ 0 },
			port{ 0 }
		{
		}

		Address::Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d,
			unsigned short port)
			//: address{ (a << 24) | (b << 16) | (c << 8) | d },
			: port{ port }
		{
			address = (a << 24) | (b << 16) | (c << 8) | d;

		}

		Address::Address(sockaddr_in address_struct)
			:
			address{ static_cast<unsigned int>(htonl(address_struct.sin_addr.S_un.S_addr)) },
			port{ ntohs(address_struct.sin_port) }
		{
		}

		Address::Address(unsigned int address, unsigned short port)
			: address{ address },
			port{ port }
		{
		}

		unsigned Address::GetAddress() const
		{
			return address;
		}

		std::string Address::GetAddressString() const
		{
			std::string add_string{
				std::to_string(GetA()) + "." +
				std::to_string(GetB()) + "." +
				std::to_string(GetC()) + "." +
				std::to_string(GetD()) };
			return add_string;
		}

		unsigned short Address::GetPort() const
		{
			return port;
		}

		struct sockaddr_in Address::GetStruct() const
		{
			struct sockaddr_in addr {};
			memset(&addr, 0, sizeof(addr));

			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			addr.sin_addr.S_un.S_addr = htonl(address);

			return addr;
		}

		unsigned char Address::GetA() const
		{
			return address >> 24;
		}

		unsigned char Address::GetB() const
		{
			return address >> 16;
		}

		unsigned char Address::GetC() const
		{
			return address >> 8;
		}

		unsigned char Address::GetD() const
		{
			return address;
		}
	}
}