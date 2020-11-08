#include "directories.h"
#include <iostream>

Directories::Directories()
{

}


unsigned int Directories::AddDirectory()
{
    dirs_.paths.push_back("");
    dirs_.drive_ids.push_back(-1);
    return dirs_.paths.size() - 1;
}


bool Directories::SetDirectoryPath(const unsigned int &id, const std::string &path)
{
    if(id > dirs_.paths.size() or id < 0 or !std::filesystem::exists(path) or not std::filesystem::is_directory(path))
        return false;
    dirs_.paths.at(id) = path;
    dirs_.drive_ids.at(id) = SetDrive(path);
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
    emit DriveChanged(drives_.names.size() - 1);
    return drives_.names.size() - 1;

}


std::string Directories::GetDirectoryPath(const unsigned int &id)
{
    return dirs_.paths.at(id);
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
