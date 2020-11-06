#include "directories.h"
#include <iostream>

Directories::Directories()
{

}


unsigned int Directories::AddDirectory(const std::string &path)
{
    if(path.empty() || !std::filesystem::exists(path) || !std::filesystem::is_directory(path))
        return -1;
    metadata_.paths.push_back(path);
    metadata_.drive_ids.push_back(SetDrive(path));
    emit DirectoryChanged(metadata_.paths.size() - 1);
    return metadata_.paths.size() - 1;
}


bool Directories::SetDirectoryPath(const unsigned int &id, std::string &path)
{
    if(id > metadata_.paths.size() || !std::filesystem::exists(path) || !std::filesystem::is_directory(path))
        return false;
    metadata_.paths.at(id) = path;
    metadata_.drive_ids.at(id) = SetDrive(path);
    emit DirectoryChanged(id);
    return true;
}


unsigned int Directories::SetDrive(const std::string &path)
{
    QStorageInfo drive(QString::fromStdString(path));

    for(unsigned int i = 0; i < drives_.names.size(); i++)
        if(drives_.names.at(i) == drive.displayName())
            return i;

    std::filesystem::space_info space = std::filesystem::space(path);
    drives_.names.push_back(drive.rootPath());
    drives_.sizes_free.push_back(space.available);
    drives_.sizes_total.push_back(space.capacity);
    return drives_.names.size() - 1;

}


std::string Directories::GetDirectoryPath(const unsigned int &id)
{
    return metadata_.paths.at(id);
}





void Directories::PrintAllDrives()
{
    for(unsigned int i = 0; i < drives_.names.size(); i++)
    {
        std::cout << "Drive " << drives_.names.at(i).toStdString() << std::endl;
        std::cout << "TiB total: " << drives_.sizes_total.at(i) * byte_tib_ << std::endl;
        std::cout << "TiB free: "  << drives_.sizes_free.at(i) * byte_tib_ << std::endl;
        std::cout << "GiB total: " << drives_.sizes_total.at(i) * byte_gib_ << std::endl;
        std::cout << "GiB free: "  << drives_.sizes_free.at(i) * byte_gib_ << std::endl;
        std::cout << "__________"  << std::endl;
    }
}
