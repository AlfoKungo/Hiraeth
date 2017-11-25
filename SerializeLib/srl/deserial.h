#pragma once

#include <cereal/archives/binary.hpp>

namespace SRL {
	template <class Rtype>
	static Rtype deserial(const char* datafile_name, unsigned int address)
	{
			std::ifstream file(datafile_name, std::ios::in | std::ios::binary);
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