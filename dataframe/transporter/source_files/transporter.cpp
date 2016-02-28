//transporter.cpp
#ifndef TRANSPORTER
#define TRANSPORTER

template<typename DataframeT>
class transporter{
public:

/* typedefs */
	
	//Serialized data to be sent over MPI
	typedef char* Inventory;

/* methods */
	template<typename IteratorT>
	static void PackRows(
			Inventory 	inventory,
			DataframeT 	data,
			IteratorT 	iterator,
			);
	
	//Sends rows to the dataframe at the destination
	template<typename CommunicatorT, typename IteratorT, typename AddressT>
	static void SendRows(
			DataFrameT 	source,
			AddressT	destination,
			CommunicatorT 	communicator,
			IteratorT 	iterator	
			);

	//Moves a dataframe to new destination
	template<typename AddressT>
	static void MoveDataframe(
			DataframeT data,
			AddressT destination
			);

private:

/* methods */

	//Recieves rows for dataframe in serialized form
	template<typename CommunicatorT, typename AddressT>
	static void RecieveRows(
			AddressT 	source,
			Inventory 	inventory,
			CommunicatorT 	communicator 
			);

	//Used by dataframes to alert that they moved in
	template<typename AddressT>
	static void RegisterDataframe(AddressT *address);

	//Used by dataframes to alert that they moved out
	template<typename AddressT>
	static void DeregisterDataframe(AddressT *address);

};

#endif
