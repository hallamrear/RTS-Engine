#pragma once
#include <filesystem>
#include <string>

namespace Bennett
{
	namespace FileManagement
	{	
		bool OpenFileDialog(const char* extensionFilter, std::string& path);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="extensionFilter">Extension of the filetype you want to return. Includes the dot e.g. ".pngs", ".jpgs".</param>
		/// <param name="searchPath">The base folder path to search</param>
		/// <param name="checkAllFolders">If true, it will check inside every child folder.</param>
		/// <param name="listToPopulate">The vector that fills with the found paths.</param>
		/// <returns>Whether or not the path exists.</returns>
		bool GetListOfFilePathsWithExtension(const std::string& extensionFilter, const std::string& searchPath, const bool& checkAllFolders, std::vector<std::filesystem::path>& listToPopulate);
	};
}
