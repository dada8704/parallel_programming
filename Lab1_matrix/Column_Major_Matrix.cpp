#include<iostream>
#include<vector>
#include<string>
#include<math.h>
#include<algorithm>
#include<time.h>

#include "Matrix.hpp"

template<typename T>
class Column_Major_Matrix
{
//private:
//    std::vector<std::vector<T>> all_column;
public:
    std::vector<std::vector<T>> all_column;
    //constructor
    Column_Major_Matrix(const int cols = 3, const int rows = 3)
    {
        srand(time(0));
        all_column.resize(cols);
        for(auto& col : all_column)
        {
            col.resize(rows);
            std::generate(col.begin(), col.end(), rand);
            for(auto& row : col)
                row = fmod((row / 100), 10);
        }
    }
    //destroctor
    ~Column_Major_Matrix() = default;

    //copy construction
    Column_Major_Matrix(const Column_Major_Matrix& x) : all_column(x.all_column) {  }
    
    //copy assignment
    Column_Major_Matrix& operator= (const Column_Major_Matrix& x)
    {
        this->all_column = x.all_column;
        return *this;
    }

    //move construction
    Column_Major_Matrix(Column_Major_Matrix&& rhs) : all_column(std::move(rhs.all_column)) {  }

    //move assigment
    Column_Major_Matrix& operator= (const Column_Major_Matrix&& rhs)
    {
        if(this != &rhs)
            all_column = std::move(rhs.all_column);
        return *this;
    }
    
    //getter function
    auto getter(int idx)
    {
        return all_column[idx];
    }
};

int main(void)
{
    Column_Major_Matrix<int> test2;
    test2.all_column[2][0] = 8;
    Column_Major_Matrix <int> test = test2;
    //test = std::move(test2);
    test.all_column[0][2] = 9;
    std::cout << test.all_column.size() << std::endl;
    for(auto& col : test2.all_column)
    {
        for(auto& row : col)
            std::cout << row << " ";
        std::cout << std::endl;
    }
    for(auto col : test2.getter(0))
    {
        std::cout << col << " ";
    }
    std::cout << std::endl;
    return 0;
}
