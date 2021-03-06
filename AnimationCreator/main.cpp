// AnimationCreator.cpp : Defines the entry point for the console application.
//
#include "srl/texture_data.h"
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <fstream>
#include <filesystem>
#include "maths/vec2.h"
#include "srl/sprite_data.h"

namespace fs = std::experimental::filesystem;

void differentAlgorithm()
{
		//{
		//	for (int y = 0; y < (MaxHeight); ++y)
		//	{
		//		int x_location = 0;
		//		for (const auto& elmt : td_vec)
		//		{
		//			for (int rel_x = 0; rel_x < (elmt.width * 4); rel_x++)
		//			{
		//				if (y*(elmt.width * 4) + rel_x < elmt.width * elmt.height * 4)
		//				td.pic[y * (4 * FullWidth) + x_location] = elmt.pic[y*(elmt.width * 4) + rel_x];
		//				x_location++;
		//			}
		//		}
		//	}
		//}
}


void createAnimationInPath(std::string path_to_directory, std::string animation_type)
{
	int FullWidth = 0, MaxHeight = 0;
	std::map<int, SRL::TextureData> td_vec;

	// accomulate textures
	for (auto & p : fs::directory_iterator(path_to_directory))
	{
		std::string fname = p.path().filename().replace_extension("").string();
		std::string path = p.path().string();
		if (path.find(animation_type) != std::string::npos && path.find("json") == std::string::npos && fname != animation_type)
		{
			SRL::TextureData td;
			td.load_texture(path);
			FullWidth += td.width;
			MaxHeight = std::max(MaxHeight, td.height);
			std::string index = fname.substr(fname.rfind(".") + 1);
			td_vec[std::stoi(index)] = td;
			std::cout << path << " of size " << maths::vec2{ float(td.width) , float(td.height) } << std::endl;
		}
	}

	// fill data
	SRL::TextureData td;
	td.width = FullWidth;
	td.height = MaxHeight;
	td.pic = new BYTE[FullWidth * MaxHeight * 4];
	for (int i = 0; i < FullWidth * MaxHeight * 4; ++i)
		td.pic[i] = 0;
	SRL::AnimationData animation_data{ {}, false };
	int x_location = 0;
	for (const auto &[key, elmt] : td_vec)
	{
		const SRL::SpritedFrameData frame_data{
			{ float(x_location), 0, float(elmt.width), float(elmt.height) },
			{ float(elmt.width) / 2.0f, 0 }, 0.09f };
		animation_data.frames_data.push_back(frame_data);
		for (int y = 0; y < (MaxHeight); ++y)
			for (int x = 0; x < (elmt.width * 4); x++)
				if (x + y * 4 * elmt.width < elmt.width * elmt.height * 4)
					td.pic[(x_location + y * FullWidth) * 4 + x] = elmt.pic[(y * 4 * elmt.width) + x];
		x_location += elmt.width;
	}

	std::ofstream data_file(path_to_directory + "/" + animation_type + ".json", std::ios::out);
	cereal::JSONOutputArchive arout(data_file);
	arout(CEREAL_NVP(animation_data));

	SRL::save_image(td.pic, (path_to_directory + "/" + animation_type + ".png").c_str(), td.width, td.height);
}

void iterate_over_folders(std::vector<std::string> animations_types, std::string srl_type, std::string extension)
{
	
		for (auto & p : fs::directory_iterator("../Serializer/data/" + srl_type + extension))
			for (const auto & at : animations_types)
				for (auto & sp : fs::directory_iterator(p))
				{
					std::string filename{ sp.path().filename().replace_extension("").string() };
					if (filename.find(at) != std::string::npos && filename != at)
					{
						createAnimationInPath(sp.path().parent_path().string(), at);
						break;
					}

				}
}

int main()
{
	iterate_over_folders({ "effect", "hit", "ball" }, "skills", "");
	iterate_over_folders({ "die" }, "monster", "/textures");

	//std::cout << FullWidth << std::endl;
	std::cin.get();
	return 0;
}

