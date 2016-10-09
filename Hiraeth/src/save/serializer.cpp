#include "serializer.h"

namespace hiraeth {
	namespace graphics {

		Serializer::Serializer()
		{

#define SAVE 1
	std::string file("my.save");
#if SAVE


	int one;
		ifstream fin((file).c_str(), ios::binary);
		fin.read((char *)&one, sizeof(one));
#else
	int one = 40;
		std::ofstream fout((file).c_str(), ios::binary);
		fout.write((char *)&one, sizeof(one));
#endif
		}
		Serializer::~Serializer()
		{

		}
	}
}