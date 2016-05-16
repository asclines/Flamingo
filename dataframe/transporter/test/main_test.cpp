#include <gtest/gtest.h>

#if defined(TRANSPORTER_TEST)
#include "transporter_test.cpp"
#endif

class TransportEnvironment : public testing::Environment{
public: 
	virtual ~TransportEnvironment() {}
	   // Override this to define how to set up the environment.
	virtual void SetUp() {}
		    //     // Override this to define how to tear down the environment.
	virtual void TearDown() {}

};
		    



int main(int argc, char **argv){
	::testing::InitGoogleTest(&argc,argv);

	::testing::Environment* const env = ::testing::AddGlobalTestEnvironment( new TransportEnvironment);

	return RUN_ALL_TESTS();
}
