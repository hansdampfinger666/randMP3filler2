#include "directories.h"
#include <iostream>

Directories::Directories()
{

}

unsigned int Directories::add_directory(const std::string &path)
{
    if(path.empty() || !std::filesystem::exists(path) || !std::filesystem::is_directory(path))
        return -1;
    metadata_.paths.push_back("");
    metadata_.disk_ids.push_back(set_disk(path));
    return metadata_.paths.size() - 1;
}

bool Directories::set_directory_path(const unsigned int &id, std::string &path)
{
    if(id > metadata_.paths.size() || !std::filesystem::exists(path) || !std::filesystem::is_directory(path))
        return false;
    metadata_.paths.at(id) = path;
    metadata_.disk_ids.at(id) = set_disk(path);
    return true;
}

unsigned int Directories::set_disk(const std::string &path)
{
    //avaible space = usable

    // get  unique identifier for moutpoint to
    auto space = std::filesystem::space(path);

    return 1;
}
