#ifndef RANDOM_H
#define RANDOM_H

#pragma once
#include <x_pch.h>

class Random
{
public:
    Random() = default;
    ~Random() = default;
    int GetRandom(const int range_from, const int range_to);

private:

    int instances_ = 0;
    std::vector<int> ranges_from_ = {};
    std::vector<int> ranges_to_ = {};
    std::vector<std::mt19937> generators_ = {};
    std::vector<std::uniform_int_distribution<int>> distributions_ = {};

    int NewRandomizer(const int range_from, const int range_to);
    int GetRandomizer(const int range_from, const int range_to);
};

#endif // RANDOM_H
