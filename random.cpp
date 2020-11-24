#include "random.h"


int Random::GetRandom(const int range_from, const int range_to)
{
    int i = GetRandomizer(range_from, range_to);
    if(i == -1)
        i = NewRandomizer(range_from, range_to);
    return distributions_.at(i)(generators_.at(i));
}


int Random::GetRandomizer(const int range_from, const int range_to)
{
    for(int i = 0; i < instances_ - 1; i++)
        if(ranges_from_.at(i) == range_from and ranges_to_.at(i) == range_to)
            return i;
    return -1;
}


int Random::NewRandomizer(const int range_from, const int range_to)
{
    instances_++;
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int> distribution(range_from, range_to);
    ranges_from_.push_back(range_from);
    ranges_to_.push_back(range_to);
    generators_.push_back(generator);
    distributions_.push_back(distribution);
    return instances_ - 1;
}






