#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QDir>
#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <error.h>


class Config : public QObject
{
    Q_OBJECT

public:
    typedef struct{
      std::vector<std::string> tokens;
      std::vector<std::string> values;
    } Params;

    Config();
    int AddParam(const std::string &param_token);
    void ReadConfig(int &ec);
    const std::string GetParam(const int param_index);


private:
    static constexpr const char file_name_lnx_[] = "/config";
    std::string path_;
    Params params_;

    bool ValidateConfigLine(std::string &line);
    void Trim(std::string &str);
};

#endif // CONFIG_H
