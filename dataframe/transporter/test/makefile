# FOR USE ON SCHOENBERG CLUSTER ONLY
export PATH := mpibin:$(PATH)

#Number of processes to test on
NP=5

# Compiler macros
CC=mpic++
FLAGS=-std=c++11 -pg -O0
DB=-g
LIBS=-lgtest -lpthread

# Includes
IN_T= -I ../source_files/
IN_MPI=-isystem /usr/include/openmpi-x86_64

# Libraries


# Directories
SRC=../source_files

# Files
F_TEST=./*.cpp
T_TEST=$(SRC)/*

run: clean
	mpirun -n $(NP) test_all

runlog: clean
	mpirun -n $(NP) test_all >> test.out

clean:
	@(rm *.out ) &> /dev/null || true

test: $(F_TEST) $(T_TEST)
	$(CC) $(FLAGS) $(LIBS) $(IN_T) -o test_all main_test.cpp  $(IN_MPI) $(DB)


