#ifndef MATRIX_SRC_MATRIX_H
#define MATRIX_SRC_MATRIX_H

#include <algorithm>
#include <iostream>



template <typename T, size_t H, size_t W>
class matrix {
private:
    T **mtrix;

    template<typename A , size_t h , size_t w>
    friend bool operator==(const matrix<A,h,w>& , const matrix<A,h,w>& );
    /// You should use this only inside det() method once.
    using matrix_minor = matrix<T, std::max<size_t>(H - 1, 1), std::max<size_t>(W - 1, 1)>;
public:
    explicit matrix(){
        mtrix = new T *[H];
        for (size_t i = 0 ; i < H ; i++)
            mtrix[i] = new T [W];
        for (size_t i = 0 ; i < H ; i++)
            for (size_t j = 0 ; j < W ; j++)
                mtrix[i][j] = 0;
    }

    matrix (const T& num) : mtrix(){
        mtrix = new T *[H];
        for (size_t i = 0 ; i < H ; i++)
            mtrix[i] = new T [W];
        for (size_t i = 0 ; i < H ; i++)
            for (size_t j = 0 ; j < W ; j++)
                mtrix[i][j] = num;
    }

    matrix (const matrix& other ) : mtrix(){
        mtrix = new T *[H];
        for (size_t i = 0 ; i < H ; i++)
            mtrix[i] = new T [W];
        for (size_t i = 0 ; i < H ; i++)
            for (size_t j = 0 ; j < W ; j++)
                mtrix[i][j] = other.mtrix[i][j];
    }
    ~matrix (){
        for (size_t i = 0 ; i < H ; i++)
            delete[] mtrix[i];
        delete[] mtrix;
    }
    matrix& operator= (const matrix& other){
        for (size_t i = 0 ; i < H ; i++)
            for (size_t j = 0 ; j < W ; j++)
                mtrix[i][j] = other.mtrix[i][j];
        return *this;
    }

    matrix& operator+= (const matrix<T,H,W>& other){
        for (size_t i = 0 ; i < H ; i++)
            for (size_t j = 0 ; j < W ; j++)
                mtrix[i][j] += other.at(i,j);
        return *this;
    }
    matrix& operator+= (const T& num){
        for(size_t i = 0 ; i < H ; i++)
            for (size_t j = 0; j < W ; j++)
                mtrix[i][j]+=num;
        return *this;
    }

    matrix& operator-= (const matrix<T,H,W>& other){
        for (size_t i = 0 ; i < H ; i++)
            for (size_t j = 0 ; j < W ; j++)
                mtrix[i][j] -= other.at(i,j);
        return *this;
    }
    matrix& operator-= (const T& num){
        for(size_t i = 0 ; i < H ; i++)
            for (size_t j = 0; j < W ; j++)
                mtrix[i][j]-=num;
        return *this;
    }

    const T& at(const size_t& i , const size_t& j) const {
        return mtrix[i][j];
    }
    T& at(const size_t& i , const size_t& j){
        return mtrix[i][j];
    }

    matrix& operator*= (const T& t){
        for (size_t i = 0 ; i < H ; i++)
            for (size_t j = 0; j < W ; j++)
                mtrix[i][j] *=t;
        return *this;
    }

    const matrix operator+() const{
        return *this;
    }

    const matrix operator-() const{
        for (size_t i = 0 ; i < H ; i++)
            for (size_t j = 0; j < W ; j++)
                mtrix[i][j] = -mtrix[i][j];
        return *this;
    }

    matrix<T,W,H> transposed() const{
        matrix<T,W,H> temp;
        for (size_t i = 0 ; i < H ; i++)
            for (size_t j = 0 ; j < W ; j++)
                temp.at(j,i)=mtrix[i][j];
        return temp;
    }

    T trace() const{
        T _trace = 0;
        for (size_t i = 0 ; i < H ; i++)
            _trace += mtrix[i][i];
        return _trace;
    }

    matrix& operator*= (const matrix<T,W,W>& other){
        matrix temp(*this);
        for (size_t i = 0 ; i < H ; i++)
            for (size_t j = 0; j < W ; j++)
                mtrix[i][j] = 0;

        for(size_t i = 0 ; i < H ; i++)
            for(size_t j = 0 ; j < W ; j++)
                for(size_t k = 0 ; k < W; k++)
                    mtrix[i][j] +=  temp.at(i,k)*other.at(k,j);
        return *this;
    }


    T det() const{
        T temp = 0;
        if( H==1 )
            return mtrix[0][0];

        for ( size_t i = 0; i < H; i++) {
            matrix_minor New(0);

            for(size_t j = 0; j < H-1 ;j++)
                for(size_t t = 0 ; t < i ;t++)
                    New.at(j,t)=mtrix[j+1][t];

            for(size_t z = 0; z < H-1 ; z++)
                for(size_t y = i ; y < H+1 ; y++)
                    New.at(z,y)=mtrix[z+1][y+1];


            temp+=((i%2==0)? 1:-1)*mtrix[0][i]* New.det();

        }
        return temp;
    }

};

/// ++++
template<typename T , size_t H , size_t W>
matrix<T,H,W> operator+ (const matrix<T,H,W>& first , const matrix<T,H,W>& second) {
    return matrix<T,H,W>(first)+=second;
}
template<typename T , size_t H , size_t W>
matrix<T,H,W> operator+ (const matrix<T,H,W>& first , const T& num) {
    return matrix<T,H,W>(first)+=num;
}
template<typename T , size_t H , size_t W>
matrix<T,H,W> operator+ ( const T& num , const matrix<T,H,W>& first ) {
    return matrix<T,H,W>(first)+=num;
}

/// ----
template<typename T , size_t H , size_t W>
matrix<T,H,W> operator- (const matrix<T,H,W>& first , const matrix<T,H,W>& second) {
    return matrix<T,H,W>(first)-=second;
}
template<typename T , size_t H , size_t W>
matrix<T,H,W> operator- (const matrix<T,H,W>& first , const T& num) {
    return matrix<T,H,W>(first)-=num;
}
template<typename T , size_t H , size_t W>
matrix<T,H,W> operator- ( const T& num , const matrix<T,H,W>& first ) {
    return matrix<T,H,W>(first)-=num;
}


/// comparition
template<typename A , size_t h , size_t w>
bool operator== (const matrix<A,h,w>& first , const matrix<A,h,w>& second){
    for (size_t i = 0 ; i < h ; i++)
        for (size_t j = 0 ; j < w ; j++)
            if (first.at(i,j) != second.at(i,j))
                return false;
    return true;
}
template<typename A , size_t h , size_t w>
bool operator!=(const matrix<A,h,w>& first , const matrix<A,h,w>& second){
    return !(first==second);
}

/// ****
template<typename A , size_t h , size_t w>
matrix<A,h,w> operator* (const matrix<A,h,w>& first , const A& t){
    matrix<A,h,w> temp(first);
    for (size_t i = 0 ; i < h ; i++)
        for ( size_t j = 0 ; j < w ; j++)
            temp.at(i,j) *=t;
    return temp;
}
template<typename A , size_t h , size_t w>
matrix<A,h,w> operator* ( const A& t , const matrix<A,h,w>& first ){
    matrix<A,h,w> temp(first);
    for (size_t i = 0 ; i < h ; i++)
        for ( size_t j = 0 ; j < w ; j++)
            temp.at(i,j) *=t;
    return temp;
}
template<typename A , size_t h , size_t w , size_t t>
matrix<A,h,t> operator* ( const matrix<A,h,w>& first , const matrix<A,w,t>& second){
    matrix<A,h,t> temp;
    for(size_t i=0;i<h;i++)
        for(size_t j=0;j<t;j++)
            for(size_t k=0;k<w;k++)
                temp.at(i,j)+=first.at(i,k)*second.at(k,j);
    return temp;
}



#endif /// MATRIX_SRC_MATRIX_H.
