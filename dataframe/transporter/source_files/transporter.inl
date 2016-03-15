#include "transporter.cpp"

template<typename DataframeT>
template<typename IteratorT>
static void Transporter<DataframeT>::PackRows(
		Inventory       inventory,
		DataframeT      *source_data,
		IteratorT       iterator,
		AddressKey	*source_key,
		AddressKey 	*destination_key
		){
//TODO implement


}

template<typename DataframeT>
static void Transporter<DataframeT>::SendInventory(
		DataframeT 	*source_data,
		AddressKey 	*destination_key,
		Inventory 	inventory
		){

	int destination; //TODO determine how to get that from address
	int tag; //TODO determine how to utilize this

	_communicator.send(destination,tag,inventory);

};
