#ifndef _ROW_MAJOR_MATRIX_HPP_
#define _ROW_MAJOR_MATRIX_HPP_

#include<iostream>
#include<vector>
#include<string>
#include<math.h>
#include<algorithm>
#include<assert.h>
#include<thread>
#include<time.h>

template<typename T>
class Column_Major_Matrix;


template<typename T>
class Row_Major_Matrix 
{
private:
    int Rows = 0, Cols = 0;
    std::vector<std::vector<T>> all_row;
public:
    //constructor
    Row_Major_Matrix(std::vector<std::vector<T>> x) : all_row(x), Rows(x.size()), Cols(x[0].size()) { }
    Row_Major_Matrix(const int rows = 3, const int cols = 3) : Rows(rows), Cols(cols)
    {
        srand(time(0));
        all_row.resize(rows);
        for(auto& row : all_row)
        {
            row.resize(cols);
            std::generate(row.begin(), row.end(), rand);
            for(auto& col : row)
                col = fmod((col / 100), 10);
        }
    }
    //destroctor
    ~Row_Major_Matrix() = default;

    //copy construction
    Row_Major_Matrix(const Row_Major_Matrix& x) : all_row(x.all_row) {  }
    
    //copy assignment
    Row_Major_Matrix& operator= (const Row_Major_Matrix& x)
    {
        this->all_row = x.all_row;
        return *this;
    }

    //move construction
    Row_Major_Matrix(Row_Major_Matrix&& rhs) : all_row(std::move(rhs.all_row)) {  }

    //move assigment
    Row_Major_Matrix& operator= (const Row_Major_Matrix&& rhs)
    {
        if(this != &rhs)
            all_row = std::move(rhs.all_row);
        return *this;
    }
    
    
    //getter function
    const auto& getter(int idx) const
    {
        return all_row[idx];
    }
    auto get_all() const
    {
        return all_row;
    }

    //setter function
    void setter(int idx, std::vector<T> vec)
    {
        assert(vec.size() == Cols);
        all_row[idx] = vec;
    }

    //****************
    template<typename U>
    Row_Major_Matrix<T> operator* (const Column_Major_Matrix <U>& CC)
    {
        std::vector<std::vector<T>> vec( Rows, std::vector<T> (CC.get_all().size(), 0));
        std::cout << "fuck: Rows: " << Rows << " CC.getter(0).size(): " << CC.getter(0).size() << std::endl;
        assert(Cols == CC.getter(0).size());
        for(int i = 0; i < Rows; ++i)
            for(int j = 0; j < CC.get_all().size(); ++j)
                for(int k = 0; k < Cols; ++k)
                    vec[i][j] += all_row[i][k] * CC.getter(j)[k];
        
        return Row_Major_Matrix (vec) ;
    }
    template<typename U>
    void Row_Mul(int idx, int last, std::vector<std::vector<T>>& ans, Column_Major_Matrix<U>& CCC)
    {
        auto CC = CCC.get_all();
        std::cout << "Cols: " << Cols << "  CC[0].size(): " << CC[0].size() << "\n";
        assert(Cols == CC[0].size());
        for(int i = idx; i < last; ++i)
            for(int j = 0; j < CC.size(); ++j)
                for(int k = 0; k < Cols; ++k)
                    ans[i][j] += all_row[i][k] * CC[j][k];
    }

    //type-cast operator
    template<typename U>
    Row_Major_Matrix<T> operator% (Column_Major_Matrix<U>& CC)
    {
        int thread_num = 10;
        std::cout << "GGGGGGG:     " << Rows << ",   " << CC.get_all().size() << "\n";
        std::vector<std::vector<T>> final_matrix ( Rows, std::vector<T> (CC.get_all().size(), 0));
        std::vector<std::thread> job;
        int space = Rows / (thread_num - 1);
        for(int i = 0; i < thread_num - 1; ++i)
            job.emplace_back(&Row_Major_Matrix<T>::Row_Mul<T>, this, i * space, (i + 1) * space, std::ref(final_matrix), std::ref(CC));
        job.emplace_back(&Row_Major_Matrix<T>::Row_Mul<T>, this, (thread_num - 1) * space, Rows, std::ref(final_matrix), std::ref(CC));
        for(int i = 0; i < thread_num; ++i)
            job[i].join();
        return Row_Major_Matrix (final_matrix);
    }

    operator Column_Major_Matrix<T>()
    {
        std::vector<std::vector<T>> vec (Cols,  std::vector<T> (Rows, 0) );
        for(auto i = 0; i < Cols; ++i)
            for(auto j = 0; j < Rows; ++j)
                vec[i][j] = all_row[j][i];
                
        return Column_Major_Matrix (vec);
    }
};
/*
int main(void)
{
    Row_Major_Matrix <int> test2;
    Row_Major_Matrix <int> test = test2;
    std::cout << test.get_all().size() << std::endl;
    for(auto& row : test2.get_all())
    {
        for(auto& col : row)
            std::cout << col << " ";
        std::cout << std::endl;
    }
    test2.setter(0, {11, 12, 13});
    for(auto row : test2.getter(0))
    {
        std::cout << row << " ";
    }
    std::cout << std::endl;
    return 0;
}*/

#endif
