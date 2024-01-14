#include "CustomUtility.hpp"

int displayCallback(void *, int argc, char **argv, char **colNames)
{
    for (int i = 0; i < argc; ++i)
        cout << colNames[i] << " = " << (argv[i] ? argv[i] : "NULL") << "\t";
    cout << '\n';
    return 0;
}

int generateIntRandom(unsigned int &seed, const unsigned int min, const unsigned int max)
{
    mt19937 generator(seed);
    uniform_int_distribution<int> distribution(min, max);
    seed++;
    return distribution(generator);
}