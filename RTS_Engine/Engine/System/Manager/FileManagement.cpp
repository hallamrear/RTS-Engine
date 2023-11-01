#include <BennettPCH.h>
#include <System/Manager/FileManagement.h>
#include <System/ServiceLocator.h>
#include <Rendering/Window.h>
#include <commdlg.h>
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
}