//transporter.cpp
#ifndef TRANSPORTER
#define TRANSPORTER

namespace mpi = boost::mpi;

template<typename DataframeT>
class Transporter{
public:

/* typedefs */
	
	//Serialized data to be sent over MPI
	typedef char* Inventory;
	typedef unsigned int AddressKey;

/* methods */
	template<typename IteratorT>
	static void PackRows(
			Inventory 	inventory,
			DataframeT 	*source_data,
			IteratorT 	iterator,
			AddressKey	*address_key
			);

	//Sends pre-packed rows (Inventory) to the dataframe at the destination
	static void SendInventory(
			DataframeT 	*source_data,
			AddressKey 	*destination_key,
			Inventory 	inventory
			);
			

	//Takes dataframe rows, packs them and sends them to the dataframe at the destination 
	template<typename IteratorT>
	static void SendRows(
			DataFrameT 	*source,
			AddressKey	*destination_key,
			IteratorT 	iterator	
			);


private:

/* methods */

	//Recieves rows for dataframe in serialized form
	static void RecieveRows(
			AddressKey	*source_key,
			Inventory 	inventory
			);

/* data members */
	mpi::environment _environment;
	mpi::communicator _communicator;

};

#include "transporter.inl"
#endif
