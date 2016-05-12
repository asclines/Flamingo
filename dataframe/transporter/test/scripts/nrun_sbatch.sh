#! /bin/sh

# Runs run_sbatch.sh with nohup

echo "Running in background"


nohup scripts/run_sbatch.sh &

echo "Started"

