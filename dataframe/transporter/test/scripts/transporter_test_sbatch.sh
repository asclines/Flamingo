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
DIR=/home/clines.alexander/GitRepos/MyRepos.d/Flamingo/dataframe/transporter/test

/usr/lib64/openmpi/bin/mpirun  $DIR/test_all #--gtest_filter="*Simple*"
