#ifndef PERMUTATION_SRC_PERMUTATION_H
#define PERMUTATION_SRC_PERMUTATION_H

#include <algorithm>
#include <iostream>
template <size_t N>
class permutation{
private:
    unsigned int *arr;

public:
    //Constructor permutation();
    permutation() : arr(new unsigned [N]) {
        for ( size_t i = 0 ; i < N ; i++)
            arr[i] = i;
    }

    //Constructor 2nd
    explicit  permutation (const unsigned *array) : arr(new unsigned[N]) {
        for (size_t i = 0 ; i < N ; i++)
            arr[i] = array[i];
    }

    permutation(const permutation &other) : arr(new unsigned [N]){
        if (&other != this)
            for (size_t i = 0 ; i < N ; i++)
                arr[i] = other.arr[i];
    }

    ~permutation() {
        delete[] arr;
    }

    permutation& operator=(const permutation& other){
        if (this != &other)
            for (size_t i = 0 ; i < N ; i++)
                arr[i] = other.arr[i];

        return *this;
    }

    permutation& operator*=(const permutation& other){
        permutation<N> temp;
        for (size_t i = 0 ; i < N ; i++)
            temp.arr[i] = arr[other.arr[i]];
        for (size_t i = 0 ; i < N ; i++)
            arr[i] = temp[i];
        return *this;
    }

    permutation next() const {
        permutation copy(*this);
        unsigned temp = N;

        int z = -1;
        for (ssize_t i = N - 2; i >= 0; i--) {
            if (copy.arr[i] < copy.arr[i + 1]) {
                z = i;
                break;
            }
        }
        ssize_t index = z;
        for (ssize_t t = N - 1; t > z; t--)
            if (copy.arr[t] > copy.arr[z] && copy.arr[t] <= temp) {
                temp = copy.arr[t];
                index = t;
            }
        if ( z != -1 )
            std::swap(copy.arr[z], copy.arr[index]);
        for (size_t j = 0; j < (N - 1 - z) / 2; j++)
            std::swap(copy.arr[z + 1 + j], copy.arr[N - 1 - j]);
        return copy;
    }

    permutation prev() const {
        permutation copy(*this);
        unsigned temp = 0;

        int z = -1;
        for (ssize_t i = N - 2; i >= 0; i--) {
            if (copy.arr[i] > copy.arr[i + 1]) {
                z = i;
                break;
            }
        }
        size_t index = z;
        for (ssize_t t = N - 1; t > z; t--)
            if (copy.arr[t] < copy.arr[z] && copy.arr[t] >= temp) {
                temp = copy.arr[t];
                index = t;
            }
        if (z!=-1)
            std::swap(copy.arr[z], copy.arr[index]);
        for (size_t j = 0; j < (N - 1 - z) / 2; j++)
            std::swap(copy.arr[z + 1 + j], copy.arr[N - 1 - j]);
        return copy;

    }

    const unsigned& operator[] (unsigned index) const {
        return arr[index];
    }

    permutation inverse() const {
        permutation<N> copy;
        unsigned index;

        for ( unsigned t = 0 ; t < N ; t++) {
            for ( unsigned i = 0 ; i < N ; i++){
                if (arr[i] == t ){
                    index = i;
                    copy.arr[t] = index;
                }
            }
        }
        return copy;
    }

    void operator() (unsigned *values ) const {
        unsigned* temp = new unsigned[N];

        for (size_t t = 0 ; t < N ; t++)
            for (size_t i = 0 ; i < N ; i++){
                if (arr[i] == t){
                    temp[t] = values[i];
                }
            }

        for (size_t i = 0 ; i < N ; i++){
            values[i] = temp[i];
        }
        delete[] temp;
    }
    permutation& operator++ () { // ++a
        permutation temp((*this).next());
        for (size_t i = 0; i < N; i++)
            arr[i] = temp[i];
        return *this;
    }
    permutation& operator-- () {
        permutation temp((*this).prev());
        for (size_t i = 0; i < N; i++)
            arr[i] = temp[i];
        return *this;
    }
    const permutation operator++ (int) { //a++
        permutation copy(*this);
        operator++();
        return copy;
    }
   const permutation operator-- (int) {
        permutation copy(*this);
        operator--();
        return copy;
    }
private:
    template<size_t a>
    friend bool operator< (const permutation<a>& , const permutation<a>&);
};

template<size_t a>
bool operator< (const permutation<a>& first , const permutation<a>& second ){
    for (unsigned i = 0 ; i < a ; i++){
        if (first.arr[i] < second.arr[i])
            return true;
        if (first.arr[i] > second.arr[i])
            return false;
    }
    return false;
}

template<size_t a>
bool operator> (const permutation<a>& first , const permutation<a>& second ){
    return second < first;
}

template<size_t a>
bool operator>= (const permutation<a> first , const permutation<a> second ){
    return !(first < second);
}

template<size_t a>
bool operator<= (const permutation<a> first , const permutation<a> second ){
    return !( second < first);
}

template<size_t a>
bool operator== (const permutation<a> first , const permutation<a> second ){
    return !(first < second) && !(second < first);
}

template<size_t a>
bool operator!= (const permutation<a> first , const permutation<a> second ){
    return (first < second) || (second < first);
}

template<size_t a>
permutation<a> operator* (const permutation<a> first, const permutation<a> second){
    return permutation<a>(first) *= second;
}

int main() {
    permutation<100> a;
    permutation<100> d(a.prev());

    for (size_t i = 0; i < 100; i++)
        std::cout << d[i] << "\n";
}
#endif /// PERMUTATION_SRC_PERMUTATION_H.
