#ifndef _COLUMN_MAJOR_MATRIX_HPP_
#define _COLUMN_MAJOR_MATRIX_HPP_

#include<iostream>
#include<vector>
#include<string>
#include<math.h>
#include<algorithm>
#include<thread>
#include<assert.h>
#include<time.h>

template<typename T>
class Row_Major_Matrix;

//#include "Row_Major_Matrix.hpp"
template<typename T>
class Column_Major_Matrix
{
private:
    int Cols = 0, Rows = 0;
    std::vector<std::vector<T>> all_column;
public:
//    std::vector<std::vector<T>> all_column;
    //constructor
    Column_Major_Matrix(std::vector<std::vector<T>> x) : all_column(x), Rows(x[0].size()), Cols(x.size()) { }
    Column_Major_Matrix(const int rows = 3, const int cols = 3) : Rows(rows), Cols(cols)
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
    auto getter(int idx) const 
    {
        return all_column[idx];
    }
    auto get_all() const
    {
        return all_column;
    }

    //setter function
    void setter(int idx, std::vector<T> vec)
    {
        assert(vec.size() == Rows);
        all_column[idx] = vec;
    }

    //******************
    template<typename U>
    Column_Major_Matrix<T> operator* (const Row_Major_Matrix<U> & RR)
    {
        std::vector<std::vector<T>> vec( Rows, std::vector<T> (RR.getter(0).size(), 0));
        for(int i = 0; i < Rows; ++i)
            for(int j = 0; j < RR.getter(0).size(); ++j)
                for(int k = 0; k < Cols; ++k)
                    vec[i][j] += all_column[k][i] * RR.getter(k)[j];
        return Column_Major_Matrix (vec);
    }

    template<typename U>
    void Col_Mul(int idx, int last, std::vector<std::vector<T>>& ans, Row_Major_Matrix<U>& RRR)
    {
        auto RR = RRR.get_all();
        std::cout << "same: " << Cols << ", " << RR.size();
        assert(Cols == RR.size());
        for(int i = idx; i < last; ++i)
            for(int j = 0; j < RR[0].size(); ++j)
                for(int k = 0; k < Cols; ++k)
                    ans[i][j] += all_column[k][i] * RR[k][j];
    }

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    template<typename U>
    Column_Major_Matrix<T> operator% (Row_Major_Matrix<U>& RR)
    {
        int thread_num = 10;
        std::vector<std::vector<T>> final_matrix (Rows, std::vector<T> (RR.getter(0).size(), 0));
        std::vector<std::thread> job;
        int space = Rows / (thread_num - 1);
        for(int i = 0; i < thread_num - 1; ++i)
            job.emplace_back(&Column_Major_Matrix<T>::Col_Mul<T>, this, i * space, (i + 1) * space,
                std::ref(final_matrix), std::ref(RR));
        job.emplace_back(&Column_Major_Matrix<T>::Col_Mul<T>, this, (thread_num - 1) * space, Rows,
            std::ref(final_matrix), std::ref(RR));
        for(int i = 0; i < thread_num; ++i)
            job[i].join();
        return Column_Major_Matrix (final_matrix);
    }


    //type-cast operator
    operator Row_Major_Matrix<T>()
    {
        std::vector<std::vector<T>> vec (Cols,  std::vector<T> (Rows, 0) );
        for(auto i = 0; i < Cols; ++i)
            for(auto j = 0; j < Rows; ++j)
                vec[i][j] = all_column[j][i];

        return Row_Major_Matrix (vec);
    }
};
/*
int main(void)
{
    Column_Major_Matrix<int> test2;
    Column_Major_Matrix <int> test = test2;
    std::cout << test.get_all().size() << std::endl;
    for(auto& col : test2.get_all())
    {
        for(auto& row : col)
            std::cout << row << " ";
        std::cout << std::endl;
    }
    test2.setter(0, {11, 12, 13});
    for(auto col : test2.getter(0))
    {
        std::cout << col << " ";
    }
    std::cout << std::endl;
    return 0;
}*/

#endif
