#include<iostream>
#include<vector>
#include<string>
#include<math.h>
#include<algorithm>
#include<assert.h>
#include<time.h>

template<typename T>
class Row_Major_Matrix 
{
private:
    int Rows = 0, Cols = 0;
    std::vector<std::vector<T>> all_row;
public:
    //constructor
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
    auto getter(int idx) const
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
};

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
}
