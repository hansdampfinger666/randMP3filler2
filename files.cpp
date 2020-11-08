#include "files.h"

Files::Files()
{

}

void Files::SetCopyList(const std::string &root_path, const int &size)
{
    if(randomizer_ == nullptr)
        randomizer_ = new Random;

    unsigned int folder_qty = CountSubfolders(root_path);

    unsigned int band_folder_randomizer = randomizer_->SetupRandomizer(0, folder_qty - 1);
    unsigned int band_folder_id, album_folder_id = -1;
    unsigned int album_folder_randomizer = randomizer_->SetupRandomizer(0,0);

    while(copy_size_ < size)
    {
        band_folder_id = randomizer_->GetRandom(band_folder_randomizer);
        std::string folder = GetFolderNameByIndex(root_path, band_folder_id);

        if(folder not_eq "")
        {
            unsigned int subfolder_qty = CountSubfolders(folder);

            if(subfolder_qty == 1)
                album_folder_id = 0;
            else
            {
                randomizer_->ResetRandomizer(album_folder_randomizer, 0, subfolder_qty - 1);
                album_folder_id = randomizer_->GetRandom(album_folder_randomizer);
            }
            std::string subfolder = GetFolderNameByIndex(folder, album_folder_id);
            int add_size = GetFileSizesInFolder(subfolder);
            if(copy_size_ + add_size < size)
              std::cout << "pusback all the things" << std::endl;
            }
    }
    delete randomizer_;
}


unsigned int Files::CountSubfolders(const std::string &path)
{
    unsigned int subfolders = 0;
    for(auto subfolder : std::filesystem::directory_iterator(path))
        subfolders++;
    return subfolders;
}


std::string GetFolderNameByIndex(const std::string &root_path, const unsigned int &id)
{
    unsigned int i = 0;
    for(auto path : std::filesystem::directory_iterator(root_path))
    {
        if(i == id and std::filesystem::is_directory(path))
            return path.path();
        i++;
    }
    return "";
}


int Files::GetFileSizesInFolder(const std::string &path)
{
    int size = 0;
    for(auto file : std::filesystem::directory_iterator(path))
        if(std::filesystem::is_regular_file(file))
                size += file.file_size();
    return size;
}
