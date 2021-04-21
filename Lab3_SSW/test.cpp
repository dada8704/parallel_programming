#include<iostream>
#include<string>
#include "ReadFasta.hpp"
#include "SmithWaterman.hpp"


int main(void)
{
    std::string hehe = "./data/Reference.fasta";
    std::string hehe1 = "./data/reference.fasta";
    Fasta rEf (ReadFasta::read(hehe));
    Fasta sEq (ReadFasta::read(hehe1));

    std::string wow = "wow";

    auto ans = SmithWaterman::SW(rEf, sEq);
    
    std::cout << ans.first << std::endl;


    return 0;
}
