#include <string>
#include <iostream>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <cmath>

#define GetCurrentDir getcwd

#define PROGRAM_NAME "pls"
#define AUTHORS proper_name("Nikhil Adiga")

namespace fs = std::filesystem;

/*
 * Method to remove directory path from the listed files/folders
 */
std::string parseString(std::string &path, std::__cxx11::basic_string<char> fileName)
{
    return fileName.replace(0, path.length() + 1, "");
}


/*
 * Method to get current directory
 */
std::string getPresentWorkDirectory()
{
    char buff[FILENAME_MAX];
    GetCurrentDir(buff, FILENAME_MAX);
    std::string current_working_dir(buff);
    return current_working_dir;
}


/*
* Method to convert bytes to human readable format
*/
std::string convertSizeToHumanReadable(long double size)
{
    long double b;
    long double kb;
    long double mb;
    long double gb;

    b = size;
    kb = b / 1024;
    mb = kb / 1024;
    gb = mb / 1024;

    if (gb > 1)
    {
        return std::to_string((int)gb) + " GB";
    }
    else if (mb > 1)
    {
        return std::to_string((int)mb) + " MB";
    }
    else if (kb > 1)
    {
        return std::to_string((int)kb) + " KB";
    }
    else
    {
        return std::to_string((int)b) + " bytes";
    }
}


/*
* Method to get size of files and directories
*/
long double getSize(std::filesystem::directory_entry entry)
{
    long double fileSize = 0;

    for (const auto &p : fs::recursive_directory_iterator(entry.path()))
    {
        if (fs::is_directory(fs::status(p.path())))
        {
            getSize(p);
        }
        else
        {
            fileSize += std::filesystem::file_size(p.path());
        }
    }

    return fileSize;
}


int main()
{
    std::string path = getPresentWorkDirectory();
    if (path.length() > 1)
    {
        for (const auto &entry : fs::directory_iterator(path))
        {
            std::cout << "\x1B[93m"
                      << "├── "
                      << parseString(path, entry.path().u8string());

            if (fs::is_directory(fs::status(entry.path())))
            {
                std::cout << " 📁" << std::endl;
                std::cout << std::setw(10)
                          << "\x1B[33m"
                          << "├── "
                          << convertSizeToHumanReadable(getSize(entry)) << std::endl
                          << std::endl;
            }
            else
            {
                std::cout << std::endl;
                std::cout << std::setw(10)
                          << "\x1B[33m"
                          << "├── "
                          << convertSizeToHumanReadable(std::filesystem::file_size(entry.path()))
                          << std::endl
                          << std::endl;
            }
        }
    }
}