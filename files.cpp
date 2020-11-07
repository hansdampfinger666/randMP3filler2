#include "files.h"

Files::Files()
{

}

void Files::SetCopyList(std::string &path)
{
    for(auto file : std::filesystem::directory_iterator(path))
        std::cout << file.path() << std::endl;

    // artist folder / album folder
    // if this is not the case, continue? what to do?

    // if this it the case



    //does folder in path have music files?
    //  no: next
    //  yes:

    // first step: get random band folder distribuation
    // second step: get random album folder distribution
}


QList<QString> Files::GetFolderList()
{

}


QList<QString> Files::GetFileList()
{

}
