
#include <filesystem>

#include "ImGuiApplication.h"
#include "imgui.h"
#include "imgui_common_tools.h"
#include "bits.h"

#include <Windows.h>
using std::string;
using std::vector;
using namespace ImGui;

namespace fs = std::filesystem;

vector<string> getDisks()
{
    vector<string> res;
    DWORD          dwSize                    = MAX_PATH;
    char           szLogicalDrives[MAX_PATH] = {0};

    DWORD dwResult = GetLogicalDriveStringsA(dwSize, szLogicalDrives);
    if (dwResult > 0 && dwResult <= MAX_PATH)
    {
        char *szSingleDrive = szLogicalDrives;
        while (*szSingleDrive)
        {
            string curDisk = szSingleDrive;
            if (curDisk.back() == '\\')
                curDisk.pop_back();
            res.push_back(curDisk);

            szSingleDrive += strlen(szSingleDrive) + 1;
        }
    }
    return res;
}

vector<fs::directory_entry> getFileList(const fs::directory_entry &dir)
{
    vector<fs::directory_entry> resList;
    if (!exists(dir))
        return resList;

    if (!dir.is_directory())
        return resList;

    fs::directory_iterator fileList(dir);
    for (auto &subFile : fileList)
        resList.push_back(subFile);
    auto compare = [](fs::directory_entry &x, fs::directory_entry &y)
    {
        if (x.is_directory() && !y.is_directory())
        {
            return true;
        }
        else if (!x.is_directory() && y.is_directory())
        {
            return false;
        }
        else
        {
            return x.path().filename() < y.path().filename();
        }
    };
    sort(resList.begin(), resList.end(), compare);

    return resList;
}

class FileBrowserApp : public ImGuiApplication
{
public:
    FileBrowserApp();
    virtual ~FileBrowserApp() {}
    virtual void presetInternal() override;
    // return if need to exit the application
    virtual bool renderUI() override;

    virtual void transferCmdArgs(std::vector<std::string> &args) override;
    virtual void dropFile(const std::vector<std::string> &files) override;

    void exit() override {}

private:
    void updateCurrDir(fs::path path);

private:
    fs::path                    mCurrDirPath;
    fs::directory_entry         mCurrDir;
    vector<fs::directory_entry> mCurrDirFileList;
};

FileBrowserApp imguiApp;

FileBrowserApp::FileBrowserApp()
{
#if defined(DEBUG) || defined(_DEBUG)
    openDebugWindow();
#endif
}

void FileBrowserApp::presetInternal()
{

    mApplicationName = "File Browser";
    updateCurrDir(fs::u8path(mExePath).parent_path());
}

void FileBrowserApp::updateCurrDir(fs::path path)
{
    if (!exists(path))
        return;

    mCurrDirPath     = path;
    mCurrDir         = fs::directory_entry(mCurrDirPath);
    mCurrDirFileList = getFileList(mCurrDir);
}

bool FileBrowserApp::renderUI()
{
    vector<string> disks     = getDisks();
    size_t         diskCount = disks.size();

    bool        dirChanged = false;
    ImGuiButton UpButton("Up");

    UpButton.show();
    if (UpButton.isClicked())
    {
        if (mCurrDirPath.has_parent_path())
        {
            fs::path parentPath = mCurrDirPath.parent_path();
            updateCurrDir(parentPath);
            dirChanged = true;
        }
        else
        {
            mCurrDirPath = "";
            mCurrDir     = fs::directory_entry();
            mCurrDirFileList.clear();
            for (int i = 0; i < diskCount; i++)
                mCurrDirFileList.push_back(fs::directory_entry(disks[i] + "\\"));
        }
    }
    SameLine();
    if (Button("Refresh") || IsKeyPressed(ImGuiKey_F5))
    {
        mCurrDirFileList = getFileList(fs::directory_entry(mCurrDirPath));
    }
    SameLine();
    Text("|");

    size_t splitPos = 0;
    size_t nextPos  = mCurrDirPath.string().find('\\', splitPos);

    if (!mCurrDirPath.empty())
    {
        while (1)
        {
            SameLine();
            if (Button(mCurrDirPath.string().substr(splitPos, nextPos - splitPos).c_str()))
            {
                updateCurrDir(mCurrDirPath.string().substr(0, nextPos));
                dirChanged = true;
                break;
            }
            if (nextPos >= mCurrDirPath.string().length() - 1)
                break;
            splitPos = nextPos + 1;
            nextPos  = mCurrDirPath.string().find('\\', splitPos);
        }
    }
    BeginChild("FileList");
    int node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth
                   | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    int tree_node_id = 0;

    if (dirChanged)
        SetScrollY(0);
    for (size_t idx = 0; idx < mCurrDirFileList.size(); idx++)
    {
        if (mCurrDirFileList[idx].is_directory())
        {
            // #00A3EFFF
            PushStyleColor(ImGuiCol_Text, bswap_32(0x00A3EFFFu));
        }
        string showStr;
        if (mCurrDirFileList[idx].path().filename().empty())
            showStr = mCurrDirFileList[idx].path().string();
        else
            showStr = mCurrDirFileList[idx].path().filename().string();
        TreeNodeEx((void *)(intptr_t)tree_node_id, node_flags, "%s", showStr.c_str());
        if (mCurrDirFileList[idx].is_directory())
        {
            PopStyleColor(1);
        }
        tree_node_id++;

        if (IsMouseDoubleClicked(ImGuiMouseButton_Left) && IsItemHovered())
        {
            if (mCurrDirFileList[idx].is_directory())
            {
                updateCurrDir(mCurrDirFileList[idx].path());
                SetScrollY(0);
            }
            else
            {
                ShellExecuteA(NULL, "open", mCurrDirFileList[idx].path().string().c_str(), NULL, NULL, SW_SHOWNORMAL);
            }
        }
    }

    EndChild();

    return false;
}

void FileBrowserApp::transferCmdArgs(std::vector<std::string> &args)
{
    // Process Command Line Arguments
}

void FileBrowserApp::dropFile(const std::vector<std::string> &files)
{
    // Process Drop File
}
