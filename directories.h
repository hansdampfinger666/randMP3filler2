#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <QObject>
#include <filesystem>

class Directories : public QObject
{
    Q_OBJECT

public:

    typedef struct{
        std::vector<std::string> paths;
        std::vector<int> disk_ids;
    } metadata;

    typedef struct{
        std::vector<std::string> paths;
        std::vector<int> sizes_total;
        std::vector<int> sizes_free;
    } disks;


    Directories();
    unsigned int add_directory(const std::string &path);
    bool set_directory_path(const unsigned int &id, std::string &path);


private:

    metadata metadata_;
    disks disks_;

    const float byte_tib_ = 0.0000000000009094947f;
    const float byte_gib_ = 0.0000000009313225728f;

    unsigned int set_disk(const std::string &path);
//    void update_disk_sizes(const unsigned int &id);

};

#endif // DIRECTORY_H
