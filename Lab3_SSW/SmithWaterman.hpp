#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include "Fasta.hpp"


class SmithWaterman
{
private:
    constexpr static int mismatch = 3, indel = 2, match = 3;
public:
    SmithWaterman() { }
    
    static std::pair<int, std::vector<int>> trace(std::vector<std::vector<int>>& dp, std::pair<int, int> end_point, std::string& ref, std::string& seq)
    {
        int i = end_point.first, j = end_point.second;
        std::vector<int> state;
        std::cout << "i: " << i << " j: " << j << "\n";
        
        for(int j = end_point.second; j > -1;)
        {
            //std::cout << "dp: " << dp[i][j] << " i: " << i << " j: " << j << "\n";
            //if(dp[i][j] == 0 || j == 0)
            //{
            //    std::cout << "dp: " << dp[i][j] << " j: " << j << "\n";
            //    break;
            //}
            if((dp[i-1][j-1] + match == dp[i][j]) && ref[i-1] == seq[j-1])//  && dp[i-1][j-1] >= dp[i][j-1])
            {
                --i, --j;
                if(j==1)
                    std::cout << ref[i] << " " << seq[j-1] << seq[j] << " " << dp[i][j] << " " << dp[i-1][j-1] << "\n";
                state.emplace_back(0);// matech
            }
            else if(dp[i-1][j] - indel == dp[i][j])
            {
                --i;
                state.emplace_back(1); // delete
            }
            else if(dp[i][j-1] - indel == dp[i][j])
            {
                --j;
                state.emplace_back(2);// insert
            }
            else if(dp[i-1][j-1] - mismatch == dp[i][j])// mismatch
            {
                --i, --j;
                state.emplace_back(3);
            }
            else if(dp[i][j] == 0 || j == 0)
            {
                std::cout << "dp: " << dp[i][j] << " j: " << j << "\n";
                break;
            }
        }
        
        return std::make_pair(i, state);
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
        std::pair<int, std::vector<int>> state = trace(dp, best.second, ref, seq);
        std::cout << "\n" << state.first << "  ans:\n\n";


        Output(ref, seq, state);

        //for(int i = 0; i < best.second.first + 1; ++i)
        //    std::cout << ref[i];
        std::cout << "\n\n";
        return best;
    }

    static void Output(std::string& ref, std::string& query, std::pair<int, std::vector<int>>& state)
    {
        std::cout << "all long: " << state.second.size() << "\n";
        int ref_iter = state.first;
        int query_iter = 0;
        for(int i = state.second.size() - 1; i > -1; --i)
        {
            if(state.second[i] == 2)
                std::cout << "-";
            else
                std::cout << ref[ref_iter++];
        }
        std::cout << "\n";
        for(int i = state.second.size() - 1; i > -1; --i)
        {
            if(state.second[i] == 0)
                std::cout << "|";
            else
                std::cout << " ";
        }
        std::cout << "\n";
        for(int i = state.second.size() - 1; i > -1; --i)
        {
            if(state.second[i] == 1)
                std::cout << "-";
            else
                std::cout << query[query_iter++];
        }
        std::cout << "\n";
        for(int i = state.second.size() - 1; i > -1; --i)
            std::cout << state.second[i];
        std::cout << "\n";
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
        
        std::pair<int, std::vector<int>> state = trace(dp, best.second, ref, seq);

        Output(ref, seq, state);
        std::cout << "\n" << state.first << "  ans:\n\n";
        for(int i = state.first; i < best.second.first + 1; ++i)
            std::cout << ref[i] ;
        std::cout << "\n\n";        
        return best; //position (start, end)


    }
};
