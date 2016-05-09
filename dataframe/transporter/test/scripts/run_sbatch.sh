#! /bin/bash

# USAGE! 
# This script is intended to be run from the test folder as follows:
# $ pwd
# ../transporter/test
# $ ./scripts/run_sbatch.sh


echo "Starting"
NN=5 # Number of nodes
NT=5 # Number of tasks
NPN=1 # Number of tasks per nodes

SAVE=0 # If 1, output logs will save to logs/

while getopts ":n:N:sh" opt; do
	case $opt in 
	n) # Number of tasks
		NT=$OPTARG
		;;
	N) # Number of nodes
		NN=$OPTARG
		;;
	s)
		SAVE=1
		;;
	h)
		echo "Help!"
		exit 1
		;;
	\?)
		echo "Invalid argument"
		exit 1
		;;
	:)
		echo "Option -$OPTARG requires an argument." 
		exit 1
		;;

	esac
done

if [ ! -f test_all ]
then
	echo "test_all does not exist!"
	exit 1
fi


echo "Cleaning files from previous runs"
make clean

#echo "Making test"
#make test

echo "Running test"
SOUT=$(sbatch -N $NN ./scripts/sbatch_transporter.sh)
SJOB=${SOUT: -3} # Gets last 3 characters of SOUT command which should be the job number # TODO switch to getting all characters after last space for X digit numbers instead of 3, just in case..

echo "Running job $SJOB"

JFLAG=0
while [ $JFLAG -ne 1 ]
do
	EQ=$(squeue | grep $SJOB)
	echo "$EQ"
	if [ "$EQ" = "" ]
	then
		echo "Program done"
		JFLAG=1
	else
		echo "Waiting for job $SJOB to finish"
		sleep 2
	fi
done

if [ $SAVE -ne 0 ] 
then 
	echo "Saving out files to logs/$SJOB"
	mkdir -p "logs/$SJOB"
	for file in *.out; do
		mv $file "logs/$SJOB/"
	done

	for file in out/*.out; do
		mv $file "logs/$SJOB/"
	done

#	if [ -f output_test.out ]
#		then
#			mv "output_test.out" "logs/slurm-"$SJOB"-out.out"
#		else 
#			echo "Warning: Expected output file not found [output_test.out]"
#	fi
#
#	if [ -f "slurm-"$SJOB".out" ]
#	then
#		mv "slurm-"$SJOB".out" "logs/slurm-"$SJOB".out"
#	else
#		echo "Warning: Expected output file not found [slurm-"$SJOB".out]"
#	fi
#
fi



echo "Done"


