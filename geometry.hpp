/*
 * geometry.hpp
 *
 *  Created on: 31 авг. 2015 г.
 *      Author: Tata
 */

#ifndef GEOMETRY_HPP_
#define GEOMETRY_HPP_
#include <cstdlib>
#include "libs.hpp"


template <typename T>
class mtrx2d{

public:



	T & operator()(const int i, const int j){
	 return (ar[i+w*j]);
	}

	mtrx2d(int h, int w, T val=0){
	this->h=h;
	this->w=w;
	for (int i =0; i< h; i++){
		for (int j =0; j< w; j++){
		ar.push_back(val);
	}
	}
	}




	vector<T> ar;
	int h;
	int w;
};




template <size_t DIM, typename T> struct vect {

	vect(T data1, T data2, T data3){
		data_[0] = data1; data_[1] = data2; data_[2] = data3;}
	vect(T data1, T data2){
		data_[0] = data1; data_[1] = data2;}
    vect() { for (size_t i=DIM; i--; data_[i] = T()); }
          T& operator[](const size_t i)       {  assert(i<DIM); return data_[i]; }
    const T& operator[](const size_t i) const {  assert(i<DIM); return data_[i]; }
    T norm() const {return (T)(sqrt((*this)*(*this)));}
    vect<DIM,T> normalize() const {return ((*this)/this->norm());}

    static vect<DIM,T>  fill(const T & val=0) {
    vect<DIM,T> ret;
    for (size_t i=DIM;i--;ret[i]=val);
    return ret;
	}

private:
    T data_[DIM];
};



template<size_t ROWS, size_t COLS, typename T> struct matrix;


template <size_t DIM, typename T> vect<DIM,T > operator - (vect<DIM,T> lhs, const vect<DIM,T>& rhs){
 for (size_t i = DIM;  i--; lhs[i]-=rhs[i]);
	 return lhs;

}

template <size_t DIM, typename T> vect<DIM,T > operator + (vect<DIM,T> lhs, const vect<DIM,T>& rhs){
 for (size_t i = DIM; i--; lhs[i]+=rhs[i]);
	 return lhs;

}


template <size_t DIM, typename T> T  operator * (vect<DIM,T>& lhs, vect<DIM,T>& rhs){
T ret=0;
	for (size_t i = DIM; i--;ret+=lhs[i]*rhs[i]);
	 return ret;

}


template <size_t DIM, typename T> vect<DIM,T>   operator * (T lhs, vect<DIM,T>& rhs){
vect<DIM,T> ret;
	for (size_t i = DIM; i--;ret[i]=lhs*rhs[i]);
	 return ret;

}


template <size_t DIM, typename T> T  operator * (vect<DIM,T>& lhs, const vect<DIM,T>& rhs){
T ret=0;
	for (size_t i = DIM; i--;ret+=lhs[i]*rhs[i]);
	 return ret;

}




template <size_t DIM, typename T> T  operator * (const vect<DIM,T>& lhs, vect<DIM,T>& rhs){
T ret=0;
	for (size_t i = DIM; i--;ret+=lhs[i]*rhs[i]);
	 return ret;

}

template <size_t DIM, typename T> T  operator * (const vect<DIM,T>& lhs,const vect<DIM,T>& rhs){
T ret=0;
	for (size_t i = DIM; i--; ret+=lhs[i]*rhs[i]);

	 return ret;

}



template<size_t DIM_N,size_t DIM, typename T>   vect<DIM_N,T> embed(const vect<DIM,T> &v,const T& fill=1){
vect<DIM_N,T> ret=vect<DIM_N,T>::fill(fill);
for (size_t i = DIM; i--; ret[i]=v[i]);
return ret;
}


template<size_t DIM_N,size_t DIM, typename T> vect<DIM_N,T> proj(const vect<DIM,T> &v) { //проекци€ вектора
    vect<DIM_N,T> ret;
    for (size_t i=DIM_N; i--; ret[i]=v[i]);
    return ret;
}

template <size_t DIM, typename T> vect<DIM,T >  operator * (vect<DIM,T> lhs , T & val){
	for (size_t i = DIM;  i--; lhs[i]*=val);
	 return lhs;

}

template <size_t DIM, typename T> vect<DIM,T >  operator / (vect<DIM,T> lhs , T val){
	for (size_t i = DIM; i--; lhs[i]/=val);
	 return lhs;

}






template <typename T> vect<3,T > vect_mult(vect<3,T> lhs,vect<3,T> rhs){
vect<3,T> ret;
ret[0]=lhs[1]*rhs[2]-lhs[2]*rhs[1];
ret[1]=lhs[2]*rhs[0]-lhs[0]*rhs[2];
ret[2]=lhs[0]*rhs[1]-lhs[1]*rhs[0];
return ret;
}


template<size_t COLS,size_t ROWS,typename Number> struct dt
{
    static Number det(const matrix<ROWS,COLS,Number>& src)
    {
        Number ret=0;
        for(size_t i=COLS;i--;)
        {
            ret+=src[0][i] * src.algAdd(0,i);
        }
        return(ret);
    }
};

template<typename T> struct dt<1,1,T>
{
    static T det(const matrix<1,1,T>& src)
    {
        return(src[0][0]);
    }
};






template<size_t ROWS, size_t COLS, typename T> struct matrix{

public:
	vect<ROWS,T> col(const size_t& idx) const{
	assert(idx<COLS);
	vect<ROWS,T> ret;
	for (size_t i =ROWS;i--;ret[i]=data_[i][idx]);
	return ret;
	}

	static size_t shift(size_t in,const size_t& val)
	{
	        return(in<val ? in : ++in);
	 }

	matrix(){}
	matrix(const matrix<ROWS,COLS,T> & src){
		for (size_t i =ROWS;i--;)
			for (size_t j =COLS;j--;){
				const T num = src[i][j];
				data_[i][j]=num;
			}
	}

	    vect<COLS,T> minimums(){
	        vect<COLS,T > ret=data_[0];
	        for(size_t i=ROWS;i--;)
	        {
	            for(size_t j=COLS;j--;)
	            {
	                ret[j]=std::min(ret[j],data_[i][j]);
	            }
	        }
	        return(ret);
	    }

	    vect<COLS,T> maximums(){
	        vect<COLS,T > ret=data_[0];
	        for(size_t i=ROWS;i--;)
	        {
	            for(size_t j=COLS;j--;)
	            {
	                ret[j]=std::max(ret[j],data_[i][j]);
	            }
	        }
	        return(ret);
	    }

	    vect<COLS,T>& operator[] (size_t index)
	     {
	         return(data_[index]);
	     }

	  const  vect<COLS,T>& operator[] (size_t index) const
		     {
		         return(data_[index]);
		     }


	    static matrix<COLS,ROWS,T> ones()
	     {
	         matrix<COLS,ROWS,T> ret;
	         for(size_t i=ROWS;i--;)
	         {
	             for(size_t j=COLS;j--;)
	             {
	                 ret[i][j]=(i==j);
	             }

	         }
	         return(ret);
	     }



	    T det() const
	        {
	            return(dt<COLS,ROWS,T>::det(*this));
	        }

	        matrix<ROWS-1,COLS-1,T> minor(size_t row,size_t col) const
	        {
	            matrix<ROWS-1,COLS-1,T> ret;
	            for(size_t i=ROWS-1;i--;)
	            {
	                for(size_t j=COLS-1;j--;)
	                {
	                    ret[i][j]=data_[ret.shift(i,row)][ret.shift(j,col)];
	                }
	            }
	            return(ret);
	        }

	        T algAdd(size_t row,size_t col) const
	         {
	             return(minor(row,col).det()*( (row+col)%2 ? -1 : 1));
	         }


	        matrix<ROWS,COLS,T> Adjacent()const
	           {
	               matrix<ROWS,COLS,T> ret;
	               for(size_t i=ROWS;i--;)
	               {
	                   for(size_t j=COLS;j--;)
	                   {
	                       ret[i][j]=algAdd(i,j);
	                   }
	               }
	               return(ret);
	           }

	        T cofactor(size_t row, size_t col) const {
	               return minor(row,col).det()*((row+col)%2 ? -1 : 1);
	           }

	        matrix<ROWS,COLS,T> adjugate() const {
	            matrix<ROWS,COLS,T> ret;
	            for (size_t i=ROWS; i--; )
	                for (size_t j=COLS; j--; ret[i][j]=cofactor(i,j));
	            return ret;
	        }

	        matrix<ROWS,COLS,T>invert_transpose() {
	        	matrix<ROWS,COLS,T> ret = adjugate();
	            T tmp = ret[0]*data_[0];
	            return ret/tmp;
	        }

	        matrix<ROWS,COLS,T> invert() {
	            return invert_transpose().transpose();
	        }

	        matrix<ROWS,COLS,T> transpose() {
	        	matrix<ROWS,COLS,T>ret;
	            for (size_t i=COLS; i--; ret[i]=this->col(i));
	            return ret;
	        }


	        matrix<ROWS,COLS,T> invertT()const
	            {
	                matrix<ROWS,COLS,T> ret=Adjacent();
	                return(ret/(ret[0]*data_[0]));
	            }

	        void setCol(const T& val,size_t col)
	           {
	               for(size_t i=ROWS;i--;)
	               {
	                   data_[i][col]=val;
	               }
	           }

	        void setCol(size_t idx, vect<ROWS,T> v)
	        {
	                assert(idx<COLS);
	                for (size_t i=ROWS; i--;)
	                {
	                        data_[i][idx]=v[i];
	                }
	        }


	        static matrix<ROWS,1,T> make_from_vec_col(const vect<ROWS,T>& v)
	        {
	                //static_assertion<COLS==1>();
	                matrix<ROWS,1,T> ret;
	                ret.setCol(0,v);
	                return ret;
	        }


private:
vect<COLS,T> data_[ROWS];
};


template<size_t R1,size_t C1,size_t C2,typename number_t>
    matrix<R1,C2,number_t> operator*(
                                     const matrix<R1,C1,number_t>& lhs,
                                     const matrix<C1,C2,number_t>& rhs
                                 )
{
    matrix<R1,C2,number_t> result;
    for (size_t i=R1; i--; )
    {
        for (size_t j=C2; j--;)
        {
              result[i][j]=lhs[i]*rhs.col(j);               //перемножить скал€рно i-тую строку левого операнда
                                                            // на j-тый столбец правого
        }
    }
    return result;
}




template<size_t ROWS,size_t COLS,typename number_t>
    vect<ROWS,number_t> operator*(
                                        const matrix<ROWS,COLS,number_t>& lhs,
                                        const vect<COLS,number_t>& rhs
                                   )
{

	matrix<COLS,1,number_t> temp = matrix<COLS,1,number_t>::make_from_vec_col(rhs);
	matrix<ROWS,1,number_t> temp2= lhs * temp;

    return temp2.col(0);
}




template<size_t ROWS,size_t COLS,typename number_t>
   matrix<COLS,ROWS,number_t> operator/(
                                             matrix<ROWS,COLS,number_t> lhs,
                                             const number_t& rhs
                                          )
{
    for (size_t i=ROWS; i--; )
    {
         lhs[i]=lhs[i]/rhs;
    }
    return lhs;
}


#endif /* GEOMETRY_HPP_ */
