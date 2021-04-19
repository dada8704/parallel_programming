#include <iostream>
#include <vector>
#include <string>
#include <utility>

class SmithWaterman
{
private:
    int fuck;
public:
    SmithWaterman() : fuck(0) { }

    static std::pair<int, int> SW(std::string ref, std::string seq)
    {
        std::vector<std::vector<int>> dp (ref.size(), std::vecot<int> (0, seq.size()));


    }
};
