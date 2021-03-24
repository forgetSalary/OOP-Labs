#include <iostream>
#include <vector>
#include <memory>

template <class T>
class Set{
    std::vector<T> set;
    int level;
public:
    Set(){
        level = 0;
    }
    Set(Set<T>& left){
        *this = left;
    }
    Set(T* _set,int arr_size){
        this->level = 0;
        for (int i = 0; i < arr_size; ++i) {
            if (!has(_set[i])){
                *this += _set[i];
                this->level++;
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
    void operator+=(T& item){
        if (!has(item)){
            set.push_back(item);
        }
    }

    Set<T>* operator+(Set<T>& left){
        Set<T>* new_set = new Set<T>;
        *new_set = Set(*this);
        new_set->level = this->level;//copy right one into new set

        int left_level = left.level;

        for (int i = 0; i < left_level; ++i) {//unite with left one
            if (!this->has(left.set[i])){
                *new_set += left.set[i];
                new_set->level++;
            }
        }
        return new_set;
    }
    Set<T>* operator*(Set<T>& left){
        Set<T>* new_set = new Set<T>;
        int left_level = left.level;

        for (int i = 0; i < left_level; ++i) {
            if (this->has(left.set[i])){
                *new_set += left.set[i];
                new_set->level++;
            }
        }
        return new_set;
    }


};


int main() {
    int arr1[] = {1,2,3};
    int arr2[] = {1,3,3};
    int arr3[] = {2,2,4,5};

    std::string strs1[] = {"foo","bar","baz"};
    std::string strs2[] = {"foo","foo","baz"};
    std::string strs3[] = {"foobar","bar","baz","foo","foo"};

    Set<int> set_int1(arr1,3);
    Set<int> set_int2(arr2,3);
    Set<int> set_int3(arr3,4);

    Set<std::string> set_str1(strs1,3);
    Set<std::string> set_str2(strs2,3);
    Set<std::string> set_str3(strs3,5);

    Set<int>* new_int_set1 = set_int2+set_int3;
    Set<int>* new_int_set2 = set_int1*set_int3;
    Set<int>* new_int_set3 = set_int2*set_int3;

    Set<std::string>* new_str_set1 = set_str1+set_str2;
    Set<std::string>* new_str_set2 = set_str2*set_str3;


    return 0;
}
