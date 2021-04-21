#pragma once

#include<iostream>
#include<string>

class Fasta
{

private:
    int CppIsGood;
    std::string seq;

public:
    Fasta(std::string input) : CppIsGood(0), seq(input) {  }
    Fasta() : CppIsGood(0), seq("NNNNNN") {  }
    std::string Get() const { return seq; }
};
