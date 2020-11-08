#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <vector>

class Random
{
public:
    Random();
    unsigned int SetupRandomizer(const int &range_from, const int &range_to);
    void ResetRandomizer(const unsigned int &id, const int &range_from_new, const int &range_to_new);
    int GetRandom(unsigned int &id);

private:
    std::vector<std::mt19937> generators;
    std::vector<std::uniform_int_distribution<int>> distributions;
};

#endif // RANDOM_H
