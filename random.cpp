#include "random.h"

Random::Random()
{
}


unsigned int Random::NewRandomizer()
{
    ranges_from_.push_back(-1);
    ranges_to_.push_back(-1);
    generators_.push_back({});
    distributions_.push_back({});
    return generators_.size() - 1;
}


void Random::SetRandomizer(const unsigned int &id, const int &range_from_new, const int &range_to_new)
{
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int> distribution(range_from_new, range_to_new);
    ranges_from_.at(id) = range_from_new;
    ranges_to_.at(id) = range_to_new;
    generators_.at(id) = generator;
    distributions_.at(id) = distribution;
}


unsigned int Random::GetRandomizer(const int &range_from, const int &range_to)
{
    for(unsigned long i = 0; i < ranges_from_.size(); i++)
        if(ranges_from_.at(i) == range_from and ranges_to_.at(i) == range_to)
            return i;
    return -1;
}


int Random::GetRandom(unsigned int &id)
{
    return distributions_.at(id)(generators_.at(id));
}
