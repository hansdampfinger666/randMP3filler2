#ifndef SERIALIZE_H
#define SERIALIZE_H

#pragma once
#include <error.h>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/access.hpp>
#include <fstream>
#include <filesystem>

class Serialize
{
public:
    template<typename T>
    static void ExportData(int &ec, T &object, const std::string &filename)
    {
        std::string path = std::filesystem::current_path() /= filename;
        std::ofstream ofs(path);

        if(ofs.is_open())
        {
            cereal::BinaryOutputArchive oarchive(ofs);
            oarchive(object);
            ofs.close();
        }
        else
            ec = Error::LogOne(2, "Serialize", __func__, "File " + path + " for deserialization could not be opened");
    };
    template<typename T>
    static void ImportData(int &ec, T &object, const std::string &filename)
    {
        std::string path = std::filesystem::current_path() /= filename;
        std::ifstream ifs(path);

        if(ifs.is_open())
        {
            cereal::BinaryInputArchive iarchive(ifs);
            iarchive(object);
            ifs.close();
        }
        else
            ec = Error::LogOne(1, "Serialize", __func__, "File " + path + " for serialization could not be opened");
    };

private:

};

#endif // SERIALIZE_H
