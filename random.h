#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <vector>

class Random
{
public:

    Random();
    int NewRandomizer(const int &range_from, const int &range_to);
    bool SetRandomizer(const int &id, const int &range_from_new, const int &range_to_new);
    int GetRandomizer(const int &range_from, const int &range_to);
    int GetRandom(const int &id);

private:
    int instances_ = 0;

    std::vector<int> ranges_from_;
    std::vector<int> ranges_to_;
    std::vector<std::mt19937> generators_;
    std::vector<std::uniform_int_distribution<int>> distributions_;
};

#endif // RANDOM_H
