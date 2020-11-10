#include "file_transfer.h"


void FileTransfer::SetCopyList(const std::string &source_path, const int &file_depth)
{
    if(copy_size_ == 0)
        return;

    int top_dir_qty = CountSubfolders(source_path);

    if(top_dir_qty == 0)
        return;

    if(randomizer_ == nullptr)
        randomizer_ = new Random;

    int randomizer_id_top_dir = randomizer_->NewRandomizer(0, top_dir_qty - 1);

    while(folders_.total_size < copy_size_ - 100000000) // 100 mb landing zone
    {
        int dir_id = randomizer_->GetRandom(randomizer_id_top_dir);
        std::string dir_path = GetSubPathNameByIndex(source_path, dir_id);

        for(int i = 1; i < file_depth - 1; i++)
        {
            int dir_qty = CountSubfolders(dir_path);
            int randomizer_id;

            if(dir_qty == 0)
                break;
            else if(dir_qty == 1)
                dir_id = 0;
            else
            {
                randomizer_id = randomizer_->GetRandomizer(0, dir_qty - 1);
                if(randomizer_id == -1)
                    randomizer_id = randomizer_->NewRandomizer(0, dir_qty - 1);
                dir_id = randomizer_->GetRandom(randomizer_id);
            }
            dir_path = GetSubPathNameByIndex(dir_path, dir_id);
        }
        if(dir_path != "")
        {
            float folder_size = GetFileSizesInFolder(dir_path);
            if(folders_.total_size + folder_size <= copy_size_ and folder_size > 0
                    and FolderContainsFiles(dir_path))
            {
                folders_.total_size += folder_size;
                folders_.paths.push_back(dir_path);
                folders_.sizes.push_back(folder_size);
            }
        }
    }
    PrintTransferList();
}


int FileTransfer::CountSubfolders(const std::string &path)
{
    int subfolders = 0;
    for(auto subfolder : std::filesystem::directory_iterator(path))
        if(std::filesystem::is_directory(subfolder))
            subfolders++;
    return subfolders;
}


std::string FileTransfer::GetSubPathNameByIndex(const std::string &root_path, const int &id)
{
    int i = 0;
    for(auto path : std::filesystem::directory_iterator(root_path))
        if(std::filesystem::is_directory(path))
        {
            if(i == id)
                return path.path();
            i++;
        };
    return "";
}


float FileTransfer::GetFileSizesInFolder(const std::string &path)
{
    int size = 0;
    for(auto file : std::filesystem::directory_iterator(path))
        if(std::filesystem::is_regular_file(file))
                size += file.file_size();
    return size;
}


bool FileTransfer::FolderContainsFiles(const std::string &path)
{
    for(auto file : std::filesystem::directory_iterator(path))
        if(std::filesystem::is_regular_file(file))
            return true;
    return false;
}


void FileTransfer::TransferFiles(const std::string &target_path)
{

}




void FileTransfer::PrintTransferList()
{
    float b_mb = 0.000001f;
    float b_gb = 0.000000001f;

    for(unsigned long i = 0; i < folders_.paths.size(); i++)
        std::cout << "Folder: " << folders_.paths.at(i) << " / Size: " << folders_.sizes.at(i) * b_mb << " Mb" << std::endl;
    std::cout << "______________" << std::endl << "TOTAL SIZE: " << folders_.total_size * b_gb << " Gb" << std::endl;
}
