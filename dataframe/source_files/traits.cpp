//traits.cpp
#ifndef DATAFRAME_TRAITS
#define DATAFRAME_TRAITS

#include "columns.cpp"
#include "iterator.cpp"
#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/size.hpp>
#include <vector>
#include <thrust/tuple.h>
#include <thrust/iterator/zip_iterator.h>

template<class ... Type>
struct traits {
	typedef boost::mpl::vector<Type...>	type_vector;	
	typedef thrust::tuple<Type...>		tuple;
	typedef thrust::zip_iterator<tuple>	zip;
 
	typedef typename boost::mpl::size<type_vector>::type size;

	typedef tuple			value_type;
	typedef value_type&		reference;
	typedef unsigned long	size_type; 
	typedef long			difference_type; 

	const int num_column=size::value;

	template<int n,typename L>
	struct column_return{
		typedef typename boost::mpl::at<type_vector,boost::mpl::int_<n> >::type base;
		typedef typename column_traits<base,L>::column* type;  
	};
};

#endif 

