#include <iostream>
#include <vector>
#include "radix.h"
#include <stdint.h>


void set_test_lab6();

template <class T>
class Set{
    std::vector<T> set;
    size_t level;
public:
    Set(){
        level = 0;
    }
    Set(Set<T>& left){
        *this = left;
    }

    Set(size_t arr_size){
        if (arr_size == set.max_size()){
            throw std::bad_alloc();
        }
        else{
            set.resize(arr_size);
            level = arr_size;
        }
    }

    Set(T* _set,size_t arr_size){
        this->level = 0;
        for (int i = 0; i < arr_size; ++i) {
            if (!has(_set[i])){
                *this += _set[i];
            }
        }
    }
    ~Set(){
        set.clear();
    }

public:
    bool has(T& item){
        int current_level = set.size();
        for (int j = 0; j < current_level; ++j) {//check value is already in set
            if(set[j]==item){
                return 1;
            }
        }
        return 0;
    }

    void resize(size_t new_size){
        set.resize(new_size);
    }

    T& operator[](size_t index){
        if (index > this->level){
            throw std::out_of_range("Access violation");
        }
        else{
            return set[index];
        }
    }

    void operator+=(T& item){
        if (this->level+1 > set.max_size()){
            throw std::bad_alloc();
        }
        else{
            if (!has(item)){
                set.push_back(item);
                level++;
            }
        }
    }


    Set<T>* operator+(Set<T>& left){
        Set<T>* new_set = new Set<T>;
        *new_set = Set(*this);

        if ((this->level+left.level) > new_set->set.max_size()){
            throw std::bad_alloc();
        }
        else{
            new_set->resize(this->level+left.level);
            new_set->level = this->level;//copy right one into new set

            int left_level = left.level;

            for (int i = 0; i < left_level; ++i) {//unite with left one
                if (!this->has(left.set[i])){
                    *new_set += left.set[i];
                }
            }
            return new_set;
        }
    }
    Set<T>* operator*(Set<T>& left){
        Set<T>* new_set = new Set<T>;
        int left_level = left.level;

        for (int i = 0; i < left_level; ++i) {
            if (this->has(left.set[i])){
                *new_set += left.set[i];
            }
        }
        return new_set;
    }


};

void test_set(){
    int arr1[] = {1,2,3};
    int arr2[] = {1,3,3};
    int arr3[] = {2,2,4,5};

    std::string strs1[] = {"foo","bar","baz"};
    std::string strs2[] = {"foo","foo","baz"};
    std::string strs3[] = {"foobar","bar","baz","foo","foo"};

    Set<int> set_int1(arr1,3);
    Set<int> set_int2(arr2,3);
    Set<int> set_int3(arr3,4);

    try{
        set_int3[5];
    } catch (std::out_of_range& err) {
        std::cout << err.what();
    }

    Set<int> big_set(185185183);
    try{
        int i = 0;
        while (true){
            big_set += i;
            i++;
        }
    } catch (std::bad_alloc& err) {
        std::cout << err.what();
    }

    Set<std::string> set_str1(strs1,3);
    Set<std::string> set_str2(strs2,3);
    Set<std::string> set_str3(strs3,5);

    Set<int>* new_int_set1 = set_int2+set_int3;
    Set<int>* new_int_set2 = set_int1*set_int3;
    Set<int>* new_int_set3 = set_int2*set_int3;

    try{
        Set<std::string>* new_str_set1 = set_str1+set_str2;
        Set<std::string>* new_str_set2 = set_str2*set_str3;
    } catch (std::bad_alloc& err) {
        std::cout << err.what();
    }
}

void test_radix(){
    std::string test_values[] = {"1234.4321","11.11","10","12.",".13",".","0.0","foo.bar","789.0","0.658","9.9"};
    //std::string test_values[] = {"789.0","0.658","9.9"};//stress
    std::string result;

    int testes_count = sizeof(test_values)/sizeof(std::string);

    for (int i = 0; i < testes_count; ++i) {
        std::cout << "Oct: "<< test_values[i] << '\n';
        result.clear();
        try {
            radix_convert(test_values[i],result, 8, 6);
            std::cout << "Hex:" << result << "\n\n";
        }catch (std::exception& err) {
            std::cout << "Err:" << err.what() << "\n\n";
        }
    }
}



int main() {
    set_test_lab6();
    return 0;
}
