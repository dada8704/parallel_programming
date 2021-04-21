#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include "Fasta.hpp"


class SmithWaterman
{
private:
    constexpr static int mismatch = 2, indel = 1, match = 2;
public:
    SmithWaterman() { }
    
    static int trace(std::vector<std::vector<int>>& dp, std::pair<int, int> end_point)
    {
        int i = end_point.first, j = end_point.second;
        std::cout << "i: " << i << " j: " << j << "\n";
        
        for(int j = end_point.second; j > -1;)
        {
            //std::cout << "dp: " << dp[i][j] << " i: " << i << " j: " << j << "\n";
            if(dp[i][j] == 0 || j == 0) 
                break;
            else if(dp[i-1][j-1] + match == dp[i][j])//  && dp[i-1][j-1] >= dp[i][j-1])
                --i, --j;
            else if(dp[i-1][j] >= dp[i][j-1])
                --i;
            else
                --j;
        }
        
        return i;
    }

    static std::pair<int, std::pair<int, int>> SSW(Fasta Ref, Fasta Seq)
    {
        auto ref = Ref.Get();
        auto seq = Seq.Get();
        std::vector<std::vector<int>> dp (ref.size() + 1, std::vector<int> (seq.size() + 1, 0));
        std::pair<int, std::pair<int, int>> best (std::make_pair(0, std::make_pair(0, 0))); // score, (start, end)
        int space = (ref.size() + 15) / 16;
        for(int space_i = 0; space_i < 16; ++space_i)
        {
            int til = (space_i != 15) ? (space_i + 1) * space : ref.size();
            for(int i = space_i * space; i < til; ++i)
            {
                for(int j = 0; j < seq.size(); ++j)
                {
                    if(ref[i] == seq[j])
                    {
                        dp[i+1][j+1] = dp[i][j] + match;
                        if(dp[i+1][j+1] > best.first)
                            best = std::make_pair(dp[i+1][j+1], std::make_pair(i, j));
                    }
                    else if(dp[i+1][j] > dp[i][j+1])
                    {
                        if((dp[i+1][j] - indel) > (dp[i][j] - mismatch))
                            dp[i+1][j+1] = dp[i+1][j] - indel;
                        else
                            dp[i+1][j+1] = dp[i][j] - mismatch;
                    }
                    else
                    {
                        if((dp[i][j+1] - indel) > (dp[i][j] - mismatch))
                            dp[i+1][j+1] = dp[i][j+1] - indel;
                        else
                            dp[i+1][j+1] = dp[i][j] - mismatch;
                    }
                }       
            }
        }
        auto start = trace(dp, best.second);
        std::cout << "\n" << start << "  ans:\n\n";

        for(int i = start; i < best.second.first + 1; ++i)
            std::cout << ref[i];
        std::cout << "\n\n";
        return best;
    }
    
    static std::pair<int, std::pair<int, int>> SW(Fasta Ref, Fasta Seq)
    {
        auto ref = Ref.Get();
        auto seq = Seq.Get();
        std::vector<std::vector<int>> dp (ref.size() + 1, std::vector<int> (seq.size() + 1, 0));
        std::pair<int, std::pair<int, int>> best (std::make_pair(0, std::make_pair(0, 0))); // score, (start, end)
        std::cout << ref.size() << " " << seq.size() << "\n";
        for(int i = 0; i < ref.size(); ++i)
        {
            for(int j = 0; j < seq.size(); ++j)
            {
                if(ref[i] == seq[j])
                {
                    dp[i+1][j+1] = dp[i][j] + match;
                    if(dp[i+1][j+1] > best.first)
                        best = std::make_pair(dp[i+1][j+1], std::make_pair(i, j));
                }
                else if(dp[i+1][j] > dp[i][j+1])
                {
                    if((dp[i+1][j] - indel) > (dp[i][j] - mismatch))
                        dp[i+1][j+1] = dp[i+1][j] - indel;
                    else
                        dp[i+1][j+1] = dp[i][j] - mismatch;
                }
                else
                {
                    if((dp[i][j+1] - indel) > (dp[i][j] - mismatch))
                        dp[i+1][j+1] = dp[i][j+1] - indel;
                    else
                        dp[i+1][j+1] = dp[i][j] - mismatch;
                }
            }

        }
        
        auto start = trace(dp, best.second);

        std::cout << "\n" << start << "  ans:\n\n";
        for(int i = start; i < best.second.first + 1; ++i)
            std::cout << ref[i] ;
        std::cout << "\n\n";        
        return best; //position (start, end)


    }
};
