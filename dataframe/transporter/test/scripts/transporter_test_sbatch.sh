#!/bin/sh

# NOTE!
# This script is intended to be run frmo the test folder as follows:
# $ pwd
# ../transporter/test
# $ ./scripts/sbatch_transporter.sh
#
#
# USAGE
# sbatch -N 5 sbatch_transporter.sh # runs 5 processes
#
# 


/usr/lib64/openmpi/bin/mpirun  test_all #--gtest_filter="*Test2*"
