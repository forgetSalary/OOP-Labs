#include <set>
#include <iostream>

#define SET_SIZE 50

template<class C>
class xSet : public std::set<C>{
public:
    xSet<C>* operator*(xSet<C>& right){
        xSet<C>* new_set = new xSet;

        auto it = right.cbegin();
        while (it != right.cend()) {
            auto search = this->find(*it);
            if(search != this->cend()){
                new_set->insert(*it);
            }
            it++;
        }

        return new_set;
    }
};

template<class C>
std::ostream& operator<<(std::ostream& os,xSet<C> set){
    auto it = set.cbegin();
    while (it != set.cend()) {
        os << *it << ',';
        it++;
    }
    os << '\n';
    return os;
}

static void rand_int_set(xSet<int>& set,int len){
    for (int i = 0; i < len; ++i) {
        set.insert(rand()%len);
    }
}

void set_test_lab6(){
    xSet<int> A,B;

    srand(time(NULL));

    rand_int_set(A,SET_SIZE);
    rand_int_set(B,SET_SIZE);

    xSet<int>* C = A*B;

    std::cout << "Set A:" << A <<
                 "Set B:" << B <<
                 "Set C:" << *C;

    delete C;
}