#ifndef TRANSPORTER_TEST_STRUCTURE
#define TRANSPORTER_TEST_STRUCTURE

#include <gtest/gtest.h>
#include "transporter.hpp"
#include "logutils.hpp"

#include <string>
#include <cstring>
#include <iostream>
#include <ctime>
#include <climits>

#define TRTEST(name)\
	TEST_F(TransporterTest,name)

class TransporterTest : public ::testing::Test{

protected:

	typedef Transporter::sizev sizev;

	static void SetUpTestCase(){
		
	}

	virtual void SetUp(){

	}

	virtual void TearDown(){

	}

	static Transporter transporter;
	int world_size;
	int rank;
	std::string process_name;
	std::string test_name;
	TransportLogger logger;
	bool is_max;

	void Init(std::string name){
		test_name = name;
		world_size = transporter.GetProcessInfo().world_size;
		rank = transporter.GetProcessInfo().world_rank;
		process_name = transporter.GetProcessInfo().name;
		is_max = false;

		#if defined(TRANSPORTER_MAX_TEST)
			is_max = true;
			std::cout << "Max test!" << std::endl;
		#endif



		logger.SetProcessInfo(
				world_size,
				rank,
				process_name
				);
		std::string log_path("mkdir -p out/");
		log_path.append(test_name);
		system(log_path.c_str());
		logger.SetLogToFile(test_name + "/node-" + std::to_string(rank));
		transporter.logger.SetLogToFile(test_name 
				+ "/node-" 
				+ std::to_string(rank)
				+  "-transporter")
			;
		logger.ToggleLogFile(true);
		transporter.logger.ToggleLogFile(false);
	}


	void CheckWindow(){
		ASSERT_TRUE(transporter.window_base_addr_[0] == 'X') 
			<< "Window base address not initialized";
	}

};

Transporter TransporterTest::transporter;

#endif
