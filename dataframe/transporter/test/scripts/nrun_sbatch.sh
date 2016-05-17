#! /bin/sh

# Runs run_sbatch.sh with nohup

echo "Running in background"


nohup /home/clines.alexander/GitRepos/MyRepos.d/Flamingo/dataframe/transporter/test/scripts/run_sbatch.sh -s & 

#nohup ./scripts/run_sbatch.sh -s &

echo "Started"

