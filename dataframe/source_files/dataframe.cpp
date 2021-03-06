//dataframe.cpp
#ifndef DATAFRAME
#define DATAFRAME

#include <location.cu> 
#include "traits.cpp"
#include "columns.cpp"
#include "iterator.cpp"
#include <vector>
#include <array>
#include <iterator>
#include "dataframe_base.cpp"
#include "dataframe_traits.cpp"
namespace Flamingo{
namespace DataFrame{

template<class ... Type>
class dataframe : public dataframeBase{	

	//typedefs
	typedef dataframeBase		base;

	public:
	typedef traits<Type...>					Traits; 

	typedef typename dataframeBase::Key		ID; 	
	typedef typename Traits::size_type			size_type;
	typedef typename Traits::difference_type	difference_type;
	typedef typename Traits::value_type		value_type;
	typedef typename Traits::reference			reference;
	typedef typename Traits::const_reference	const_reference;
	typedef typename Traits::pointer			pointer;
	typedef typename Traits::const_pointer		const_pointer;
	typedef typename Traits::zip_iterator		zip_iterator;
	typedef typename Traits::const_zip_iterator	const_zip_iterator; 
	typedef dataframe_iterator<	reference,
							pointer,
							Type...>		iterator;
	typedef dataframe_iterator<	const_reference,
							const_pointer,
							Type...>		const_iterator;

	typedef std::reverse_iterator<iterator>		reverse_iterator; 
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator; 

	typedef typename Traits::type_vector			type_vector;
	typedef typename column_tuple<Type...>::type		ColumnTuple;
	public:
	dataframe();
	dataframe(const dataframe<Type...>& other):
				dataframeBase(other),
				_column_tuple(other.tuple_const()){};
	dataframe(size_type,value_type);
	dataframe(size_type);
	dataframe(iterator,iterator);

	~dataframe(); 

	public:
	void unsafe_move(Memory::Region);
	Memory::Region location()const; 

	void assign(iterator,iterator);
	void assign(size_type,value_type);

	reference at(size_type);
	reference front();
	reference back();

	iterator			begin();
	const_iterator		begin()const;
	const_iterator		cbegin()const;
	reverse_iterator			rbegin();
	const_reverse_iterator		rbegin()const;
	const_reverse_iterator		crbegin()const;
	zip_iterator			begin_zip();
	const_zip_iterator		begin_zip()const;
	const_zip_iterator		cbegin_zip()const;

	iterator			end(); 
	const_iterator		end()const; 
	const_iterator		cend()const; 
	reverse_iterator			rend();
	const_reverse_iterator		rend()const;
	const_reverse_iterator		crend()const;
	zip_iterator			end_zip();
	const_zip_iterator		end_zip()const;
	const_zip_iterator		cend_zip()const;

	size_type size()const;
	size_type device_size()const; 
	size_type max_size()const;
	bool empty()const;
	void reserve(size_type);
	size_type capacity()const;

	void clear();
	iterator insert(iterator,value_type);
	iterator insert(iterator,iterator,iterator);

	iterator erase(iterator);
	iterator erase(iterator,iterator);

	void push_back(value_type);
	void pop_back();
	void resize(size_type);
	void resize(size_type,value_type);	
	void swap(dataframe<Type...>&); 
	
	bool operator==(const dataframe<Type...>&)const;
	bool operator!=(const dataframe<Type...>&)const;
	reference operator[](size_type);
	dataframe<Type...>& operator=(const dataframe<Type...>&);

	void broadcast(iterator);
	void scatter(iterator,std::vector<int>&); 
	
	private:
	ColumnTuple		_column_tuple;

	private:
	template<int n>
	typename column_tuple<Type...>::element<n>::type& column_access();

	ColumnTuple& tuple(); 
	const ColumnTuple& tuple_const()const;

	template<typename P>
	void pop_to_array(P,iterator)const;
	
	template<typename P>
	void push_back_from_array(P,size_type);
};

#include"dataframe.inl"
}//end datafarme
}//end flamingo
#endif 

