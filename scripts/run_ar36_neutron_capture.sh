#! /bin/bash
# number of events
NUM_EVENTS=1000000

# get the directory where this script is stored
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
GAMMA_CASCADE_DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )/../"

# make a results directory
RESULTS_DIR=$GAMMA_CASCADE_DIR/results
mkdir -p $RESULTS_DIR
# output file
OUTPUT_FILE=$RESULTS_DIR/ar36_results.txt

# locate the ar36 file
AR36_FILE=$GAMMA_CASCADE_DIR/ENDF/ar36_neutron_capture.txt

# run the main program
cd $GAMMA_CASCADE_DIR/build
echo "Running $NUM_EVENTS cascades for Ar36..."
./cascade $AR36_FILE $NUM_EVENTS $OUTPUT_FILE

cd $GAMMA_CASCADE_DIR