#include "directories.h"


int Directories::AddDirectory(){
    dirs_.paths.push_back("");
    dirs_.drive_ids.push_back(-1);
    return dirs_.paths.size() - 1;
}

bool Directories::SetDirectoryPath(const int id, const std::string &path){
    if(id > (int)dirs_.paths.size() or id < 0 or !std::filesystem::exists(path) or not std::filesystem::is_directory(path))
        return false;
    dirs_.paths.at(id) = path;
    dirs_.drive_ids.at(id) = SetDrive(path);
    emit DirectoryChanged(id);
    emit DriveForDirChanged(id);
    return true;
}

int Directories::SetDrive(const std::string &path){
    QStorageInfo drive(QString::fromStdString(path));

    for(unsigned int i = 0; i < drives_.names.size(); i++)
        if(drives_.names.at(i) == drive.displayName())
            return i;

    std::filesystem::space_info space = std::filesystem::space(path);
    drives_.names.push_back(drive.rootPath());
    drives_.sizes_used.push_back(space.capacity - space.available);
    drives_.sizes_free.push_back(space.available);
    drives_.sizes_total.push_back(space.capacity);
    return drives_.names.size() - 1;
}






std::string Directories::GetDirPath(const int dir_id){
    return dirs_.paths.at(dir_id);
}

int Directories::GetDriveToDir(const int dir_id){
    return dirs_.drive_ids.at(dir_id);
}

float Directories::GetDriveUsedSpace(const int dir_id){
    int drive_id = dirs_.drive_ids.at(dir_id);
    return drives_.sizes_used.at(drive_id);
}

float Directories::GetDriveFreeSpace(const int dir_id){
    int drive_id = dirs_.drive_ids.at(dir_id);
    return drives_.sizes_free.at(drive_id);
}

float Directories::GetDriveTotalSpace(const int dir_id){
    int drive_id = dirs_.drive_ids.at(dir_id);
    return drives_.sizes_total.at(drive_id);
}

void Directories::PrintAllDrives(){
    for(unsigned int i = 0; i < drives_.names.size(); i++){
        std::cout << "Drive " << drives_.names.at(i).toStdString() << std::endl;
        std::cout << "TiB total: " << Format::GetReadableBytes(drives_.sizes_total.at(i)) << std::endl;
        std::cout << "TiB free: "  << Format::GetReadableBytes(drives_.sizes_free.at(i)) << std::endl;
        std::cout << "GiB total: " << Format::GetReadableBytes(drives_.sizes_total.at(i)) << std::endl;
        std::cout << "GiB free: "  << Format::GetReadableBytes(drives_.sizes_free.at(i)) << std::endl;
        std::cout << "__________"  << std::endl;
    }
}
