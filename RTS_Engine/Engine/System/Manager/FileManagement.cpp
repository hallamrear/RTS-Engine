#include <BennettPCH.h>
#include <commdlg.h>
#include <filesystem>
#include <Rendering/Window.h>
#include <System/Manager/FileManagement.h>
#include <System/ServiceLocator.h>
#include <thread>

namespace Bennett
{
    bool FileManagement::OpenFileDialog(const char* extensionFilter, std::string& path)
    {
        Window& window = ServiceLocator::GetWindow();
        path = "\0";

        CHAR buffer[MAX_LOADSTRING] = { 0 };
        OPENFILENAME dialogDetails{ 0 };
        ZeroMemory(&dialogDetails, sizeof(dialogDetails));
        dialogDetails.hwndOwner = window.GetWindowHandle();
        dialogDetails.lStructSize = sizeof(dialogDetails);
        dialogDetails.lpstrFile = buffer;
        dialogDetails.nMaxFile = sizeof(buffer);
        dialogDetails.lpstrFilter = "Level Files\0*.level\0";
        dialogDetails.nFilterIndex = 1;
        dialogDetails.lpstrFileTitle = NULL;
        dialogDetails.nMaxFileTitle = 0;
        dialogDetails.lpstrInitialDir = NULL;
        dialogDetails.lpfnHook = 0;
        dialogDetails.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
        bool result = GetOpenFileName(&dialogDetails);

        if (result)
        {
            path = dialogDetails.lpstrFile;
        }
        return result;
    }

    bool FileManagement::GetListOfFilePathsWithExtension(const std::string& extensionFilter, const std::string& searchPath, const bool& checkAllFolders, std::vector<std::filesystem::path>& listToPopulate)
    {
        std::filesystem::path folderPath = searchPath;

        if (std::filesystem::exists(folderPath))
        {
            if (checkAllFolders)
            {
                for (const auto& entry : std::filesystem::recursive_directory_iterator(folderPath))
                {
                    if (entry.path().extension() == extensionFilter)
                    {
                        listToPopulate.push_back(entry.path());
                    }
                }
            }
            else
            {
                for (const auto& entry : std::filesystem::directory_iterator(folderPath))
                {
                    if (entry.path().extension() == extensionFilter)
                    {
                        listToPopulate.push_back(entry.path());
                    }
                }
            }

            return true;
        }

        return false;
    }
}