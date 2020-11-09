#include "random.h"

Random::Random()
{
}


int Random::NewRandomizer(const int &range_from, const int &range_to)
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


bool Random::SetRandomizer(const int &id, const int &range_from_new, const int &range_to_new)
{
    if(id < 0 or id > instances_ - 1)
        return false;
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int> distribution(range_from_new, range_to_new);
    ranges_from_.at(id) = range_from_new;
    ranges_to_.at(id) = range_to_new;
    generators_.at(id) = generator;
    distributions_.at(id) = distribution;
    return true;
}


int Random::GetRandomizer(const int &range_from, const int &range_to)
{
    for(int i = 0; i < instances_ - 1; i++)
        if(ranges_from_.at(i) == range_from and ranges_to_.at(i) == range_to)
            return i;
    return -1;
}


int Random::GetRandom(const int &id)
{
    return distributions_.at(id)(generators_.at(id));
}
