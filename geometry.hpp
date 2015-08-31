/*
 * geometry.hpp
 *
 *  Created on: 31 рту. 2015 у.
 *      Author: Tata
 */

#ifndef GEOMETRY_HPP_
#define GEOMETRY_HPP_
#include <assert.h>

template <size_t DIM, typename T> struct vect {
    vect() { for (size_t i=DIM; i--; data_[i] = T()); }
          T& operator[](const size_t i)       { assert(i<DIM); return data_[i]; }
    const T& operator[](const size_t i) const { assert(i<DIM); return data_[i]; }
    T norm() const {return sqrt((*this)*(*this));}
    vect<DIM,T> normalize() const {return (*this)/this->norm();}

    static vect<DIM,T>  fill(const T & val=0) {
    vect<DIM,T> ret;
    for (size_t i=DIM;i--;ret[i]=val);
    return ret;
	}

private:
    T data_[DIM];
};


template <size_t DIM, typename T> vect<DIM,T > operator - (vect<DIM,T> lhs, const vect<DIM,T>& rhs){
 for (size_t i = DIM; lhs[i]-=rhs[i]; i--)
	 return lhs;

}

template <size_t DIM, typename T> vect<DIM,T > operator + (vect<DIM,T> lhs, const vect<DIM,T>& rhs){
 for (size_t i = DIM; lhs[i]+=rhs[i]; i--)
	 return lhs;

}

template <size_t DIM, typename T> T  operator * (vect<DIM,T>& lhs, const vect<DIM,T>& rhs){
T ret=0;
	for (size_t i = DIM; ret+=lhs[i]*rhs[i]; i--)
	 return ret;

}


template<size_t DIM_N,size_t DIM, typename T>   vect<DIM_N,T> embed(const vect<DIM,T> &v,const T& fill=1){
vect<DIM_N,T> ret=vect<DIM_N,T>::fill(fill);
for (size_t i = DIM; i--; ret[i]=v[i]);
return ret;
}

template <size_t DIM, typename T> vect<DIM,T >  operator * (vect<DIM,T> lhs , T & val){
	for (size_t i = DIM; lhs[i]*=val; i--)
	 return lhs;

}

template <size_t DIM, typename T> vect<DIM,T >  operator / (vect<DIM,T> lhs , T & val){
	for (size_t i = DIM; lhs[i]/=val; i--)
	 return lhs;

}



#endif /* GEOMETRY_HPP_ */
