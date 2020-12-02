#ifndef CONFIG_H
#define CONFIG_H

#pragma once
#include <QObject>
#include <QDir>
#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <error.h>
#include <cereal/access.hpp>


class Config : public QObject
{
    Q_OBJECT

public:
    void SetAvoidLastFileList(const bool flag);

    typedef struct{
      std::vector<std::string> tokens;
      std::vector<std::string> values;
    } Params;

    Config();
    int AddParam(const std::string &param_token);
    void ReadConfig(int &ec);
    const std::string GetParam(const int param_index);


private:

    struct{
        bool avoid_last_file_list = false;
        bool pls = true;
//        unsigned int pls;
    } data_;


    const char file_name_lnx_[8] = "/config";
    std::string path_;
    Params params_;

    bool ValidateConfigLine(std::string &line);
    void Trim(std::string &str);

    friend class cereal::access;
    template<class Archive>
    void save(Archive &ar) const { ar(data_); };
    template<class Archive>
    void load(Archive &ar){ ar(data_); }
};

#endif // CONFIG_H
