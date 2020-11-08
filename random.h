#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <vector>

class Random
{
public:
    Random();
    unsigned int NewRandomizer();
    void SetRandomizer(const unsigned int &id, const int &range_from_new, const int &range_to_new);
    unsigned int GetRandomizer(const int &range_from, const int &range_to);
    int GetRandom(unsigned int &id);

private:
    std::vector<int> ranges_from_;
    std::vector<int> ranges_to_;
    std::vector<std::mt19937> generators_;
    std::vector<std::uniform_int_distribution<int>> distributions_;
};

#endif // RANDOM_H
