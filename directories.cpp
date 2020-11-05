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
    metadata_.drive_ids.push_back(set_drive(path));
    return metadata_.paths.size() - 1;
}


bool Directories::set_directory_path(const unsigned int &id, std::string &path)
{
    if(id > metadata_.paths.size() || !std::filesystem::exists(path) || !std::filesystem::is_directory(path))
        return false;
    metadata_.paths.at(id) = path;
    metadata_.drive_ids.at(id) = set_drive(path);
    return true;
}


unsigned int Directories::set_drive(const std::string &path)
{
    std::filesystem::space_info space = std::filesystem::space(path);

    // no clue how to ID a mounted partition for a given path
    for(unsigned int i = 0; i < drives_.sizes_free.size(); i++)   // just compare free sizes, very unlikely that different drives have same free bytes (easy mode)
        if(drives_.sizes_free.at(i) == space.available)
            return i;   // if it's the same drive, return drive ID

    drives_.sizes_total.push_back(space.capacity);
    drives_.sizes_free.push_back(space.available);
    return drives_.sizes_free.size() - 1; // otherwise update drive tab and return next drive id
}







void Directories::cout_all_drive_sizes()
{
    for(unsigned int i = 0; i < drives_.sizes_total.size(); i++)
    {
        std::cout << "Drive " << i << std::endl;
        std::cout << "TiB total: " << drives_.sizes_total.at(i) * byte_tib_ << std::endl;
        std::cout << "TiB free: "  << drives_.sizes_free.at(i) * byte_tib_ << std::endl;
        std::cout << "GiB total: " << drives_.sizes_total.at(i) * byte_gib_ << std::endl;
        std::cout << "GiB free: "  << drives_.sizes_free.at(i) * byte_gib_ << std::endl;
        std::cout << "__________"  << std::endl;
    }
}
