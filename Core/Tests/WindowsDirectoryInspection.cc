// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

// !!! ALL THIS CODE IS A STUPIDLY EARLY TEST !!!

#include <Cell/Scoped.hh>
#include <Cell/Collection/List.hh>
#include <Cell/StringDetails/RawString.hh>
#include <Cell/System/Entry.hh>
#include <Cell/System/Log.hh>
#include <Cell/System/Platform/Windows/Includes.h>

using namespace Cell;

const char* file = "File";
const char* directory = "Directory";

const wchar_t* ignoredDirectories[2] = { L".", L".." };

// !!! THIS IS EXCLUSIVELY FOR THE GLORIAS OF THIS WORLD !!!
// !!! IF YOU AREN'T A GLORIA, THIS WILL PROBABLY BREAK. IN THE BUSINESS WE REFER TO THIS AS A SO-CALLED "SKILL ISSUE" !!!
// !!! IF ANYONE CHANGES THIS WITHOUT MAKING IT GRAB THE ACTUAL USERNAME (or just actually fetch the absolute path less hardcode-ly) I WILL BE VERY ANNOYED !!!

// !!! SORRY !!!

// also yes I use the Documents folder for my projects I'm really funny

const size_t offsetStuff = StringDetails::RawStringSize("C:\\Users\\Gloria\\Documents\\Cell\\Project\\Content\\");

void searchFolder(Collection::List<String>& list, const String& path) {
    String path2 = path + "\\*";

    WIN32_FIND_DATAW findData;

    HANDLE finder = [path2, &findData]{
        ScopedBlock path2Wide = path2.ToPlatformWideString();
        return FindFirstFileExW(path2Wide, FindExInfoBasic, &findData, FindExSearchNameMatch, nullptr, FIND_FIRST_EX_LARGE_FETCH);
    }();

    CELL_ASSERT(finder != INVALID_HANDLE_VALUE);

    while (true) {
        const bool isDirectory = findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
        bool skip = false;
        if (isDirectory) {
            for (const wchar_t* ignored : ignoredDirectories) {
                skip = wcscmp(findData.cFileName, ignored) == 0;
                if (skip) {
                    break;
                }
            }
        }

        if (!skip) {
            String path3 = String::Format("%\\%", path, (wchar_t*)findData.cFileName);
            if (isDirectory) {
                searchFolder(list, path3);
            } else {
                list.Append(path3);
            }
        }

        const BOOL result = FindNextFileW(finder, &findData);
        if (result != TRUE && GetLastError() == ERROR_NO_MORE_FILES) {
            break;
        }

        CELL_ASSERT(result == TRUE);
    }

    FindClose(finder);
}

void CellEntry(Reference<String> parameterString) {
    (void)(parameterString);

    Collection::List<String> files;
    searchFolder(files, "C:\\Users\\Gloria\\Documents\\Cell\\Project\\Content");

    System::Log("Found % files", files.GetCount());
    for (String file : files) {
        System::Log("- %", file.Substring(offsetStuff, file.GetCount() - offsetStuff).Unwrap());
    }
}
