#include "pch.h"
#include "FileSystem.h"



namespace Yugo
{

	std::string Yugo::FileSystem::GetSolutionFolderPath()
	{
		std::string solutionPath;
		std::string filePath = __FILE__;
		std::string dirPathToFile = filePath.substr(0, filePath.rfind("\\"));
		std::size_t firstFound = dirPathToFile.find("Yugo");
		// Try second found if solution is named just as project (Yugo)
		std::size_t secondFound = dirPathToFile.find("Yugo", firstFound + 1, 4);
		if (secondFound != std::string::npos)
			solutionPath = dirPathToFile.substr(0, secondFound);
		else
			solutionPath = dirPathToFile.substr(0, firstFound);

		return solutionPath;
	}

	void Yugo::FileSystem::CreateNewFolder(const std::string& folderPath)
	{
		std::filesystem::create_directory(folderPath);
	}

	void Yugo::FileSystem::CopyFileTo(const std::string& filePathFrom, const std::string& filePathTo)
	{
		std::filesystem::copy_file(filePathFrom, filePathTo);
	}

	void Yugo::FileSystem::Delete(const std::string& path)
	{
		std::filesystem::remove_all(path);
	}

	std::unordered_map<std::string, int> Yugo::FileSystem::HashFolderTree(const std::string& path)
	{
		int id = 2; // Starting from 1, because 0 is reserved for root directory
		std::unordered_map<std::string, int> map;
		for (auto& entity : std::filesystem::recursive_directory_iterator(path))
		{
			std::string name = entity.path().filename().string();
			if (map.find(name) == map.end())
				map[name] = id++;
		}
		return map;
	}

}
