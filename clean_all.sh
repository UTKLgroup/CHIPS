
export OUTPUT_DIRECTORY=${SCRATCH}/output
export WORKING_DIRECTORY=${SCRATCH}/french_outputs

list=`ls ${WORKING_DIRECTORY} | grep CHIPS `

for directory in ${list}; do

    echo $directory

    ./clean_jobs.sh ${directory} ${directory}

done
