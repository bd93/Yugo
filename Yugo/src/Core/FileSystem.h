#pragma once



namespace Yugo
{
	struct FileSystem
	{
		using DirIter = std::filesystem::directory_iterator;
		using RecDirIter = std::filesystem::recursive_directory_iterator;
		using FilePath = std::filesystem::path;

		static std::string GetSolutionFolderPath();
		static void CreateNewFolder(const std::string& folderPath);
		static void CopyFileTo(const std::string& filePathFrom, const std::string& filePathTo);
		static void Delete(const std::string& path);
		// Recursively traverse tree and hash by name
		static std::unordered_map<std::string, int> HashFolderTree(const std::string& path);
	};

}