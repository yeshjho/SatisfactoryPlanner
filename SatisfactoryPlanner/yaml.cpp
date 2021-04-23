#include <iostream>
#include <string>
#include <filesystem>
#include <yaml-cpp/yaml.h>



struct Factory
{
	std::filesystem::path thumbnail;
	int a;
};

namespace YAML
{
	template<>
	struct convert<Factory>
	{
		static Node encode(const Factory& factory)
		{
			Node node;

			node["path"] = factory.thumbnail.generic_string();
			node["a"] = factory.a;

			return node;
		}

		static bool decode(const Node& node, Factory& factory)
		{
			if (!node.IsMap())
			{
				return false;
			}

			factory.thumbnail = node["path"].as<std::string>();
			factory.a = node["a"].as<int>();

			return true;
		}
	};
}


std::ostream& operator<<(std::ostream& os, const Factory& factory)
{
	std::cout << factory.thumbnail << " " << factory.a;
	return os;
}


int main()
{
	YAML::Node node = YAML::LoadFile("resource/data/a.yaml");

	for (const auto& n : node["buildings"]["factories"])
	{
		std::cout << n.first.as<std::string>() << std::endl;
		std::cout << n.second.as<Factory>() << std::endl;
		std::cout << std::endl;
	}
}
