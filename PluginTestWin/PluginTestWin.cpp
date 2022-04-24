// PluginTestWin.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <windows.h>
#include <string>
#include "DLLBase.h"

using namespace std;

// A list to store our DLL handles
std::vector<HINSTANCE> modules;
// The data for each file we find.
WIN32_FIND_DATA fileData;
// Find the first DLL file in out plugins folder,
// and store the data in out fileData structure.


int main()
{
    HANDLE fileHandle = FindFirstFile(R"(.\plugins\*.dll)", &fileData);

    if (fileHandle == (void*)ERROR_INVALID_HANDLE ||
        fileHandle == (void*)ERROR_FILE_NOT_FOUND) {
        // We couldn't find any plugins, lets just
        // return for now (imagine this is in main)
        return 0;
    }
    // Loop over every plugin in the folder, and store
// the handle in our modules list
    do {
        // Load the plugin. We need to condense the plugin
        // name and the path together to correctly load the
        // file (There are other ways, I won't get into it here)
        HINSTANCE temp = LoadLibrary((R"(.\plugins\)" + string(fileData.cFileName)).c_str());

        if (!temp) {
            // Couldn't load the library, continue on
            cerr << "Couldn't load library " << fileData.cFileName << "!\n";
            continue;
        }

        // Add the loaded module to our list of modules
        modules.push_back(temp);

        // Typedefs for the functions. Don't worry about the
        // __cdecl, that is for the name mangling (look up
        // that if you are interested).
        typedef  DLLBase*  (__cdecl* ObjProc)(void);
        typedef const char * (__cdecl* NameProc)(void);

        // Load the functions. This may or may not work, based on
        // your compiler. If your compiler created a '.def' file
        // with your DLL, copy the function names from that to
        // these functions. Look up 'name mangling' if you want
        // to know why this happens.
        ObjProc objFunc = (ObjProc)GetProcAddress(temp, "getObj");
        NameProc nameFunc = (NameProc)GetProcAddress(temp, "getName");
        if ( nameFunc != NULL)
        {

            // use them!
            std::cout << "Plugin " << nameFunc() << " loaded!\n";
            DLLBase* obj = objFunc();
            obj->print();
            std::cout << "\t" << obj->calc(10) << std::endl;
        }
        // Continue while there are more files to find
    } while (FindNextFile(fileHandle, &fileData));

    for (HINSTANCE hInst : modules)
        FreeLibrary(hInst);

}
