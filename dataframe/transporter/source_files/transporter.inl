#include "transporter.cpp"


/* Public Methods */

template<typename DataframeT>
template<typename IteratorT>
static void Transporter<DataframeT>::PackRows(
		Inventory       inventory,
		DataframeT      *source_data,
		AddressKey 	*destination_key,
		IteratorT       iterator
		){
//TODO implement


};

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

template<typename DataframeT>
static void Transporter<DataframeT>::SendRows(
	DataframeT 	*source_data,
	AddressKey 	*destination_key,
	IteratorT 	iterator
	){
	
	Inventory inventory;

	PackRows(
		inventory,
		source_data,
		destination_key,
		iterator
		);

	SendInventory(
		source_data,
		destination_key,
		inventory
		);
};

