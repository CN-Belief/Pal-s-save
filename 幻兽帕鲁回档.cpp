#include <iostream>
#include <cstdlib>
#include <string>
#include <windows.h>
#include <fstream> 
#include <iomanip>
#include <filesystem>

#define UNLEN 256 

using namespace std;

void deleteFolder(const string& folderPath) {
    string command = "rmdir /s /q \"" + folderPath + "\"";
    system(command.c_str());
}

void deleteFile(const string& filePath) {
    string command = "del \"" + filePath + "\"";
    system(command.c_str());
}

string getFormattedDateTime(const FILETIME& ftLastWriteTime) {
    SYSTEMTIME stUTC, stLocal;
    FileTimeToSystemTime(&ftLastWriteTime, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

    ostringstream oss;
    oss << setfill('0') << setw(4) << stLocal.wYear << "-"
        << setw(2) << stLocal.wMonth << "-"
        << setw(2) << stLocal.wDay << " "
        << setw(2) << stLocal.wHour << ":"
        << setw(2) << stLocal.wMinute; 
    
    return oss.str();
}

void copyFiles(const string& sourcePath, const string& destinationPath) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((sourcePath + "\\*").c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                string sourceFile = sourcePath + "\\" + findFileData.cFileName;
                string destinationFile = destinationPath + "\\" + findFileData.cFileName;
                CopyFile(sourceFile.c_str(), destinationFile.c_str(), FALSE);
            }
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
}

string selectFolder(const string& basePath) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((basePath + "\\*").c_str(), &findFileData);
    int index = 1;
    string selectedFolder;

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (strcmp(findFileData.cFileName, ".") != 0 && strcmp(findFileData.cFileName, "..") != 0) {
                    string folderName = findFileData.cFileName;
                    string folderPath = basePath + "\\" + folderName;
                    string modifiedDate = getFormattedDateTime(findFileData.ftLastWriteTime);
                    cout << index << ". " << folderName << " (修改日期: " << modifiedDate << ")" << endl;
                    index++;
                }
            }
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    }

    int selection;
    cout << "请输入序号选择你的存档(请根据修改日期判断)：";
    cin >> selection;

    index = 1;
    hFind = FindFirstFile((basePath + "\\*").c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (strcmp(findFileData.cFileName, ".") != 0 && strcmp(findFileData.cFileName, "..") != 0) {
                    if (index == selection) {
                        selectedFolder = findFileData.cFileName;
                        break;
                    }
                    index++;
                }
            }
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    }

    return selectedFolder;
}

string selectSubFolderInFolder(const string& folderPath) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((folderPath + "\\*").c_str(), &findFileData);
    int index = 1;
    string selectedSubFolder;

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (strcmp(findFileData.cFileName, ".") != 0 && strcmp(findFileData.cFileName, "..") != 0) {
                    cout << index << ". " << findFileData.cFileName << endl;
                    index++;
                }
            }
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    }

    int selection;
    cout << "请选择你要恢复到的存档时间：" << endl;
    cin >> selection;

    index = 1;
    hFind = FindFirstFile((folderPath + "\\*").c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (strcmp(findFileData.cFileName, ".") != 0 && strcmp                (findFileData.cFileName, "..") != 0) {
				                    if (index == selection) {
				                        selectedSubFolder = findFileData.cFileName;
				                        break;
				                    }
				                    index++;
				                }
				            }
				        } while (FindNextFile(hFind, &findFileData) != 0);
				        FindClose(hFind);
				    }
				
				    return selectedSubFolder;
				}
				
				int main() {
					system("title BiliBili:墨尘之一 本程序完全免费");
					cout<<"本程序由Bilibili墨尘之一制作并发布"<<endl<<"此程序完全免费"<<endl<<"使用该程序则表明您接受使用该程序可能存在的各种风险，如：存档丢失"<<endl<<"按下回车键进入程序"<<endl;
					getchar();
					system("cls"); 
				    char username[UNLEN + 1];
				    DWORD username_len = UNLEN + 1;
				    GetUserName(username, &username_len);
				    
				    string path = "C:\\Users\\" + string(username) + "\\AppData\\Local\\Pal\\Saved";
				
				    string selectedSaveGamesFolder = selectFolder(path + "\\SaveGames");
				
				    if (selectedSaveGamesFolder.empty()) {
				        cout << "无效的选择。" << endl;
				        return 1;
				    }
				
				    string selectedFolder = selectFolder(path + "\\SaveGames\\" + selectedSaveGamesFolder);
				
				    if (selectedFolder.empty()) {
				        cout << "无效的选择。" << endl;
				        return 1;
				    }
				
				    string cundangPath = path + "\\SaveGames\\" + selectedSaveGamesFolder + "\\" + selectedFolder;
				
				
				    string beifen = cundangPath + "\\backup";

				    string localPath = beifen + "\\local";
					system("cls");
				    string selectedSubFolder = selectSubFolderInFolder(localPath);
					deleteFile(cundangPath + "\\Level.sav");
					deleteFile(cundangPath + "\\LevelMeta.sav");
					deleteFile(cundangPath + "\\LocalData.sav");
					deleteFile(cundangPath + "\\Players\\00000000000000000000000000000001.sav");
					deleteFile(cundangPath + "\\Players\\00000000000000000000000000000002.sav");
					deleteFile(cundangPath + "\\Players\\00000000000000000000000000000003.sav");
					deleteFile(cundangPath + "\\Players\\00000000000000000000000000000004.sav");
					deleteFile(cundangPath + "\\Players\\00000000000000000000000000000005.sav");

				    string sourceFolderPath = localPath + "\\" + selectedSubFolder;
				    copyFiles(sourceFolderPath, cundangPath);
				    sourceFolderPath = beifen + "\\world" + "\\" + selectedSubFolder;
				    copyFiles(sourceFolderPath, cundangPath);
					sourceFolderPath = beifen + "\\world" + "\\" + selectedSubFolder + "\\Players";
					copyFiles(sourceFolderPath, cundangPath + "\\Players");
					system("cls");
				    cout << "恢复存档成功，请打开游戏检查存档是否已经恢复" <<endl<<"按下回车键退出程序"<<endl;
				    getchar();
				    return 0;
				}
