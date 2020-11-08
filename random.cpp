#include "random.h"

Random::Random()
{
}


unsigned int Random::SetupRandomizer(const int &range_from, const int &range_to)
{
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int> distribution(range_from, range_to);
    generators.push_back(generator);
    distributions.push_back(distribution);
    return generators.size() - 1;
}


void Random::ResetRandomizer(const unsigned int &id, const int &range_from_new, const int &range_to_new)
{
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int> distribution(range_from_new, range_to_new);
    generators.at(id) = generator;
    distributions.at(id) = distribution;
}


int Random::GetRandom(unsigned int &id)
{
    return distributions.at(id)(generators.at(id));
}
