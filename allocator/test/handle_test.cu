// free_list_test.cu
#include <Handle.cpp>
#include <location.cu>
#include <gtest/gtest.h>

#include \
    "MacroUtilities.cpp"

#include <cstddef>
#include <type_traits>
#define HANDLE_THREADS 8

class HandleTest : public ::testing::Test {
    protected:
     virtual void SetUp() {
          int offset = 0;
          std::size_t size = 8;
          Handle<int> handle_int(offset, size, base_ptr);
          handle = handle_int;
     }
     int base[2] = {0, 1};
     int* base_ptr = base;
     Handle<int> handle;

     DEFINE(BuddyOffSetTest, HANDLE_THREADS)
     DEFINE(CopyConstructorTest, HANDLE_THREADS)
     DEFINE(JoinOperatorTest, HANDLE_THREADS)
     DEFINE(DereferenceOperatorTest, HANDLE_THREADS)
     DEFINE(IndirectionOperatorTest, HANDLE_THREADS)
     DEFINE(EqualityComparableTest, HANDLE_THREADS)
     DEFINE(DefaultConstructionTest, HANDLE_THREADS)
     DEFINE(CopyAssignableTest, HANDLE_THREADS)
     DEFINE(NullablePointerTest, HANDLE_THREADS)
     DEFINE(BiderectionalTest, HANDLE_THREADS)
     DEFINE(RandomAccessTest, HANDLE_THREADS)
     DEFINE(VoidTest, HANDLE_THREADS)
     DEFINE(BoolConvertTest, HANDLE_THREADS)
     DEFINE(IfStatementTest, HANDLE_THREADS)
     DEFINE(ConvertToConstTest, HANDLE_THREADS)
     DEFINE(ConstDereferenceTest, HANDLE_THREADS)
     DEFINE(DeviceSingleTest, HANDLE_THREADS)
     DEFINE(DeviceMultipleTest, HANDLE_THREADS)
     DEFINE(MemCopyTest, HANDLE_THREADS)
	DEFINE(AssignementTest,HANDLE_THREADS)
	DEFINE(ConstTest,HANDLE_THREADS)
};
void HandleTest::ConstTest() {
	int A=10;
	const int* A_ptr=&A; 

     Handle<const int> h(A_ptr);
};
void HandleTest::ConstDereferenceTest() {
     const Handle<int> h(0, 0, base_ptr);
     *h;
};

void HandleTest::BuddyOffSetTest() {
     int buddy = handle.buddy_offset();

     EXPECT_EQ(8, buddy);
};

void HandleTest::CopyConstructorTest() {
     Handle<int> handle_new(handle);
};
void HandleTest::JoinOperatorTest() {
     Handle<int> h_1(handle);
     Handle<int> h_2(handle);

     h_1._offset = 8;
     h_1.combine(h_2);
     EXPECT_EQ(h_1._offset, 0);
     EXPECT_EQ(h_1._size, 16);
};
void HandleTest::DereferenceOperatorTest() {
     *base = 0;
     EXPECT_EQ(*handle, 0);
     *base = 1;
     EXPECT_EQ(*handle, 1);
};
void HandleTest::IndirectionOperatorTest() {
     Handle<int>* handle_ptr = &handle;
     int offset = handle_ptr->_offset;
     EXPECT_EQ(offset, 0);
};
void HandleTest::EqualityComparableTest() {
     Handle<int> handle_2(0, 2, base_ptr);
     bool test_1 = (handle_2 == handle);
     EXPECT_TRUE(test_1);

     Handle<int> handle_3(1, 2, base_ptr);
     bool test_2 = (handle_3 == handle);
     EXPECT_FALSE(test_2);

     double* base_ptr2 = new double;
     Handle<double> handle_4(0, 8, base_ptr2);
     bool test_3 = (handle_4 == handle);
     EXPECT_FALSE(test_3);
};

void HandleTest::AssignementTest(){
	Handle<int> A;
	Handle<int> B;

	A=B; 
}
void HandleTest::DefaultConstructionTest() {
     Handle<int> handle_2();
};
void HandleTest::CopyAssignableTest() {
     Handle<int> handle_2 = handle;
     bool test = (handle_2 == handle);
     EXPECT_TRUE(test);
};
void HandleTest::NullablePointerTest() {
     std::nullptr_t null;
     Handle<int> handle_2(null);
     bool test_1 = (handle_2 == null);
     EXPECT_TRUE(test_1);

     Handle<int> handle_3 = null;
     bool test_2 = (handle_3 == null);
     EXPECT_TRUE(test_2);

     EXPECT_FALSE(handle == null);
     EXPECT_FALSE(null == handle);
     EXPECT_TRUE(handle != null);
     EXPECT_TRUE(null != handle);
};
void HandleTest::BiderectionalTest() {
     Handle<int> handle_2 = handle;
     handle_2++;
     EXPECT_TRUE(handle_2 != handle);
     handle_2--;
     EXPECT_TRUE(handle_2 == handle);
     --handle_2;
     ++handle_2;
     *handle_2++;
     EXPECT_EQ(*handle_2, 1);
     *handle_2--;
     EXPECT_EQ(*handle_2, 0);
};
void HandleTest::RandomAccessTest() {
     Handle<int> handle_2 = handle;

     handle_2 += 1;
     EXPECT_EQ(*handle_2, 1);
     handle_2 -= 1;
     EXPECT_EQ(*handle_2, 0);

     Handle<int> handle_3 = handle + 1;
     EXPECT_EQ(*handle_3, 1);
     Handle<int> handle_4 = 1 + handle;
     EXPECT_EQ(*handle_4, 1);
     Handle<int> handle_5 = handle_4 - 1;
     EXPECT_EQ(*handle_5, 0);

     Handle<int> handle_6 = handle;
     handle_6 += 1;
     int dif = handle_6 - handle;
     EXPECT_EQ(dif, 1);
     EXPECT_TRUE(handle < handle_6);
     EXPECT_FALSE(handle > handle_6);
     EXPECT_TRUE(handle <= handle_6);
     EXPECT_FALSE(handle >= handle_6);

     EXPECT_EQ(handle[0], 0);
     EXPECT_EQ(handle[1], 1);
};

void HandleTest::VoidTest() { Handle_void handle_void; }

void HandleTest::BoolConvertTest() { bool test = handle; }

void HandleTest::IfStatementTest() {
     bool True = true;

     True ? handle : 0;
     !True ? handle : 0;
}

void HandleTest::ConvertToConstTest() {
     bool convertable = std::is_convertible<Handle<int>, const Handle<int>>::value;
     EXPECT_TRUE(convertable);
     convertable = std::is_convertible<Handle<int>, const Handle<int>>::value;
     EXPECT_TRUE(convertable);
};

__global__ void transferS(Handle<int> x, Handle<int> y) {
     *y = *x;
};
void HandleTest::DeviceSingleTest() {
     int x = 2;
     int y = 0;

     int* x_d;
     int* y_d;

     cudaMalloc((void**)&x_d, sizeof(int));
     cudaMalloc((void**)&y_d, sizeof(int));

     cudaMemcpy(x_d, &x, sizeof(int), cudaMemcpyHostToDevice);
     cudaMemcpy(y_d, &y, sizeof(int), cudaMemcpyHostToDevice);

     Handle<int> x_h(0, 1, x_d);
     Handle<int> y_h(0, 1, y_d);
     transferS << <1, 1>>> (x_h, y_h);
     cudaDeviceSynchronize();
     cudaMemcpy(&y, y_d, sizeof(int), cudaMemcpyDeviceToHost);
     EXPECT_EQ(x, y);
     cudaFree(x_d);
     cudaFree(y_d);
}

__global__ void transferM(Handle<int> x, Handle<int> y) {
     y[1] = x[1];
};

void HandleTest::DeviceMultipleTest() {
     int x[2] = {2, 3};
     int y[2] = {0, 1};

     int* x_d;
     int* y_d;
     int size = 2 * sizeof(int);

     cudaMalloc((void**)&x_d, size);
     cudaMalloc((void**)&y_d, size);
     cudaMemcpy(x_d, &x, size, cudaMemcpyHostToDevice);
     cudaMemcpy(y_d, &y, size, cudaMemcpyHostToDevice);

     Handle<int> x_h(0, 2, x_d);
     Handle<int> y_h(0, 2, y_d);
     transferM << <1, 1>>> (x_h, y_h);
     cudaDeviceSynchronize();
     cudaMemcpy(&y, y_d, size, cudaMemcpyDeviceToHost);
     EXPECT_EQ(x[1], y[1]);
     EXPECT_FALSE(x[0] == y[0]);
     cudaFree(x_d);
     cudaFree(y_d);
}

void HandleTest::MemCopyTest() {
     typedef location<device> Location;
     Location location;
     int size = 3 * sizeof(int);
     int* x_d;
     int* y_d;

     x_d = static_cast<int*>(location.New(size));
     y_d = static_cast<int*>(location.New(size));

     Handle<int> x_h(0, 3, x_d);
     Handle<int> y_h(0, 3, y_d);

     Handle_void x_v(x_h);
     Handle_void y_v(y_h);

     //	Location::MemCopy(x_v,y_v,3);
     Location::MemCopy(x_h, y_h, 3);

     location.Delete(x_d);
     location.Delete(y_d);
}

// clang-format off
// python:key:function=ConstTest AssignementTest MemCopyTest DeviceSingleTest DeviceMultipleTest ConstDereferenceTest ConvertToConstTest IfStatementTest BoolConvertTest VoidTest BuddyOffSetTest CopyConstructorTest JoinOperatorTest DereferenceOperatorTest IndirectionOperatorTest EqualityComparableTest DefaultConstructionTest CopyAssignableTest NullablePointerTest BiderectionalTest RandomAccessTest
// python:key:concurency=Single Threaded
// python:template=TEST_F(HandleTest,|function||concurency|){this->|function||concurency|();};
// python:start
// python:include=handle.test
#include \
    "handle.test"
// python:end
// clang-format on
#undef HANDLE_THREADS
