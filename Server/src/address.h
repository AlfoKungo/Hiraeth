#pragma once
#include <string>
#include<winsock2.h>

namespace hiraeth {
	namespace network {

		class Address
		{
		private:
			unsigned int address;
			unsigned short port;
			
		public:
			Address();

			Address(unsigned char a,
				unsigned char b,
				unsigned char c,
				unsigned char d,
				unsigned short port);

			Address(struct sockaddr_in address_struct);

			Address(unsigned int address,
				unsigned short port);

			unsigned int GetAddress() const;
			std::string GetAddressString() const;

			unsigned short GetPort() const;

			struct sockaddr_in GetStruct() const;

			unsigned char GetA() const;
			unsigned char GetB() const;
			unsigned char GetC() const;
			unsigned char GetD() const;

		};
	}
}