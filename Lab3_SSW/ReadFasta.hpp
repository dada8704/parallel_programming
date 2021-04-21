#pragma once

#include<iostream>
#include<string>
#include<fstream>

#include "Fasta.hpp"

class ReadFasta
{
private:
    int fuck;
public:
    ReadFasta() : fuck(0) { }
    static Fasta  read(std::string filename) 
    { 
        std::cout << "reading " << filename << "\n";
        std::ifstream istrm(filename);
        if(!istrm.is_open())
        {
            std::cout << "fail to open " << filename << "\n";
            exit(0);
        }
        
        
        std::string trash, seq;
        //if(std::getline(istrm, trash))
        //    std::cout << trash << "\n";
        //if(std::getline(istrm, seq))
        //    std::cout << seq << "\n";
        std::getline(istrm, trash);
        std::getline(istrm, seq);

        return Fasta(seq);  
    }
};
