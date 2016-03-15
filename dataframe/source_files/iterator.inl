//iterator.inl
#include "iterator.cpp"
#include "iterator_functors.cpp"
#include "functional"
#include <iostream>
//private functions
template<class ... Type>
dataframe_iterator<Type...>::pointer dataframe_iterator<Type...>::get_pointer() const{
	return _pointer; 
}

//recursive template functors
//nulify
template<int n,typename pointer>
struct nullify {
	void operator()(pointer& p){
		std::get<n>(p)=NULL;		
		nullify<n-1,pointer> null_r;
		null_r(p);	
	}
};
template<typename pointer>
struct nullify<0,pointer> {
	void operator()(pointer& p){
		std::get<0>(p)=NULL;	
	}
};
//increment
template<int n,typename pointer>
struct increment {
	void operator()(pointer& p){

		pointer tmp=std::get<n>(p);
		tmp++;		
		std::get<n>(p)=tmp;					
		
		increment<n-1,pointer> inc_r;
		inc_r(std::forward<pointer>(p));	
	}
};
template<typename pointer>
struct increment<0,pointer> {
	void operator()(pointer& p){	
		pointer tmp=std::get<0>(p);
		tmp++;		
		std::get<0>(p)=tmp;				
	}
};
//decrement
template<int n,typename pointer>
struct decrement {
	void operator()(pointer& p){

		pointer tmp=std::get<n>(p);
		tmp--;		
		std::get<n>(p)=tmp;					
		
		decrement<n-1,pointer> inc_r;
		inc_r(p);	
	}
};
template<typename pointer>
struct decrement<0,pointer> {
	void operator()(pointer& p){	
		pointer tmp=std::get<0>(p);
		tmp--;		
		std::get<0>(p)=tmp;				
	}
};
//arithmic-plus
template<int n,typename pointer,typename T>
struct arithmic_plus {
	void operator()(pointer&& lhs,T rhs){ 
		std::get<n>(lhs)+=rhs;
		
		arithmic_plus<n-1,pointer,T> arith_r;
		arith_r(std::forward<pointer>(lhs),rhs);	
	}
};
template<typename pointer,typename T>
struct arithmic_plus<0,pointer,T> {
	void operator()(pointer&& lhs,T rhs){
		std::get<0>(lhs)+=rhs;
	}
};
//arithmic-minus
template<int n,typename pointer, typename T>
struct arithmic_minus {
	void operator()(pointer& lhs,const T& rhs){

		std::get<n>(lhs)-=rhs;
		
		arithmic_minus<n-1,pointer,T> arith_r;
		arith_r(lhs,rhs);	
	}
};
template<typename pointer,typename T>
struct arithmic_minus<0,pointer,T> {
	void operator()(pointer& lhs,const T& rhs){
		std::get<0>(lhs)-=rhs;
	}
};

//public functions
template<class ... Type>
dataframe_iterator<Type...>::dataframe_iterator(){
	nullify<sizeof...(Type)-1,pointer> _null;
	_null(_pointer);
}

template<class ... Type>
dataframe_iterator<Type...>::dataframe_iterator(const dataframe_iterator<Type...>& other){
	_pointer=other.get_pointer();
}

template<class ... Type>
dataframe_iterator<Type...>::dataframe_iterator(
	dataframe_iterator<Type...>::ColumnTuple& tuple)
{
	typename iterator_functors::assign<traits<Type...>::_numCol-1,Type...> assigner; 
	assigner(
		std::forward<ColumnTuple>(tuple),
		std::forward<pointer>(_pointer)
	); 
}

template<class ... Type>
dataframe_iterator<Type...>::~dataframe_iterator(){}

template<class ... Type>
dataframe_iterator<Type...>& dataframe_iterator<Type...>::operator=(const dataframe_iterator<Type...>& other){
	swap(other);
	return *this;
}

template<class ... Type>
bool dataframe_iterator<Type...>::operator==(const dataframe_iterator<Type...>& other) const{
	bool result=_pointer==other.get_pointer(); 
	return result;
}

template<class ... Type>
bool dataframe_iterator<Type...>::operator!=(const dataframe_iterator<Type...>& other) const{
	bool result=!(*this==other);
	return result;
}

template<class ... Type>
bool dataframe_iterator<Type...>::operator<(const dataframe_iterator<Type...>& other) const{
	bool result=_pointer<other.get_pointer();
	return result;
} 

template<class ... Type>
bool dataframe_iterator<Type...>::operator>(const dataframe_iterator<Type...>& other) const{
	return (other < *this);
} 

template<class ... Type>
bool dataframe_iterator<Type...>::operator<=(const dataframe_iterator<Type...>& other) const{
	return !(*this > other);
} 

template<class ... Type>
bool dataframe_iterator<Type...>::operator>=(const dataframe_iterator<Type...>& other) const{
	return !(*this < other);
} 

template<class ... Type>
dataframe_iterator<Type...>& dataframe_iterator<Type...>::operator++(){
	increment<sizeof...(Type)-1,pointer> _inc;
	_inc(_pointer);

	return *this;
}

template<class ... Type>
dataframe_iterator<Type...> dataframe_iterator<Type...>::operator++(int){
	dataframe_iterator<Type...> tmp(*this);
	operator++();
	return tmp;
} 

template<class ... Type>
dataframe_iterator<Type...>& dataframe_iterator<Type...>::operator--(){
	decrement<sizeof...(Type)-1,pointer> _dec;
	_dec(_pointer);

	return *this;
} 

template<class ... Type>
dataframe_iterator<Type...> dataframe_iterator<Type...>::operator--(int){
	dataframe_iterator<Type...> tmp(*this);
	operator--();
	return tmp;
} 

template<class ... Type>
dataframe_iterator<Type...>& dataframe_iterator<Type...>::operator+=(dataframe_iterator<Type...>::size_type n){
	arithmic_plus<sizeof...(Type)-1,pointer,size_type> _arith;

	_arith(std::forward<pointer>(_pointer),n);
	return *this;
} 

template<class ... Type>
dataframe_iterator<Type...> dataframe_iterator<Type...>::operator+(dataframe_iterator<Type...>::size_type n) const{
	dataframe_iterator<Type...> tmp(*this);
	tmp+=n;
	return tmp;
} 

template<class ... Type>
dataframe_iterator<Type...>& dataframe_iterator<Type...>::operator-=(dataframe_iterator<Type...>::size_type n){
	arithmic_minus<sizeof...(Type)-1,pointer,size_type> _arith;
	_arith(_pointer,n);

	return *this;
}  

template<class ... Type>
dataframe_iterator<Type...> dataframe_iterator<Type...>::operator-(dataframe_iterator<Type...>::size_type n) const{
	dataframe_iterator<Type...> tmp(*this);
	tmp-=n;
	return tmp;
} 

template<class ... Type>
dataframe_iterator<Type...>::difference_type dataframe_iterator<Type...>:: operator-(const dataframe_iterator<Type...>& other) const{
	difference_type result=std::get<0>(_pointer)-std::get<0>(other.get_pointer());
	return result;
} 


template<class ... Type>
dataframe_iterator<Type...>::reference dataframe_iterator<Type...>::operator*(){
	
	iterator_functors::
		dereference<sizeof...(Type)-1,Type...> rec;
	return rec(std::forward<pointer>(_pointer));
}

template<class ... Type>
dataframe_iterator<Type...>::reference dataframe_iterator<Type...>::operator[](dataframe_iterator<Type...>::size_type n){
	typedef typename traits<Type...>::value_type value_type;

	value_type tmp(*this); 
	tmp+=n;
	return *tmp;
}

template<class ... Type>
template<int n>
typename traits<Type...>::Return<n>::pointer_base dataframe_iterator<Type...>::get(){
	return  std::get<n>(_pointer); 
}

















