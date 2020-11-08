#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QDir>

#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>


class Config : public QObject
{
    Q_OBJECT

public:

    typedef struct{
      std::vector<std::string> tokens;
      std::vector<std::string> values;
    } params;

    Config();
    unsigned int AddParam(const std::string &param_token);
    bool ReadConfig();
    const std::string GetParam(const unsigned int &param_index);

private:

    const std::string file_name_lnx_ = "/config";
    std::string path_;
    params params_;

    bool ValidateConfigLine(std::string &line);
    void Trim(std::string &str);
};

#endif // CONFIG_H
