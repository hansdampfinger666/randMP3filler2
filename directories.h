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
        std::vector<int> drive_ids;
    } metadata;

    typedef struct{
        std::vector<unsigned long> sizes_total;
        std::vector<unsigned long> sizes_free;
    } drives;


    Directories();
    unsigned int add_directory(const std::string &path);
    bool set_directory_path(const unsigned int &id, std::string &path);

    void cout_all_drive_sizes();


private:

    metadata metadata_;
    drives drives_;

    const float byte_tib_ = 0.0000000000009094947f;
    const float byte_gib_ = 0.0000000009313225728f;

    unsigned int set_drive(const std::string &path);

};

#endif // DIRECTORY_H
